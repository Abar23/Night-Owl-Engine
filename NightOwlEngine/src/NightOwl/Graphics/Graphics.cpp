

#include "NightOwlPch.h"

#include "Graphics.h"
#include "Debugging/DebugSystem.h"
#include "Materials/Material.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Window/WindowApi.h"
#include "System/LightSystem.h"
#include "System/MeshRendererSystem.h"
#include "Types/AccessType.h"
#include "Types/GraphicsFormat.h"

namespace NightOwl
{
	void Graphics::Initialize()
	{
		IWindow* window = WindowApi::GetWindow().get();
		CreateContext(window);

		// Create G-Buffer and add initial position color and depth buffers
		deferredGBuffer = CreateRenderTexture(window->GetHeight(), window->GetWidth(), TextureFilterMode::Point, GraphicsFormat::RGBA32F, GraphicsFormat::Depth32F);
		
		// add normal color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);
		// add color + specular color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);
		// add view space position buffer for ambient occlusion
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);
		// add view space normal buffer for ambient occlusion
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);

		// Create ambient obscurrance and blur render texture with one channel each
		ambientObscurranceRenderTexture = CreateRenderTexture(window->GetHeight(), window->GetWidth(), TextureFilterMode::Point, GraphicsFormat::R32F);
		ambientObscurranceBlurredRenderTexture = CreateRenderTexture(window->GetHeight(), window->GetWidth(), TextureFilterMode::Point, GraphicsFormat::R32F);

		// Create bloom render texture
		bloomFrameBufferRenderTexture = CreateRenderTexture(window->GetHeight(), window->GetWidth(), TextureFilterMode::Point, GraphicsFormat::RGBA32F);
		bloomFrameBufferRenderTexture->AddColorAttachment(GraphicsFormat::RGBA32F);

		// Created texture that will store the HDR rendered scene
		renderedSceneRenderTexture = CreateRenderTexture(window->GetHeight(), window->GetWidth(), TextureFilterMode::Point, GraphicsFormat::RGBA32F, GraphicsFormat::Depth32F);

		// Setup render quad for gbuffer lighting calc
		const std::vector<Vec3F> VERTICES = {
			{ -1.0f, -1.0f, 0.0f }, // bottom left
			{ -1.0f,  1.0f, 0.0f }, // top left
			{  1.0f, -1.0f, 0.0f }, // bottom right
			{  1.0f,  1.0f, 0.0f }  // top right
		};

		const std::vector<Vec2F> UVS = {
			{ 0.0f, 0.0f }, // bottom left
			{ 0.0f, 1.0f }, // top left
			{ 1.0f, 0.0f }, // bottom right
			{ 1.0f, 1.0f }  // top right
		};

		const std::vector<Vec3UI> INDICES = {
			{ 0, 2, 1 },
			{ 1, 2, 3 }
		};

		quadMesh = std::make_unique<Mesh>();
		quadMesh->SetVertices(VERTICES);
		quadMesh->SetUVs(UVS);
		quadMesh->SetTriangles(INDICES);
		quadMesh->UploadMeshData(true);

		hammersleyPairsGraphicsBuffer = CreateGraphicsBuffer(BufferType::Storage);
		hammersleyPairsGraphicsBuffer->SetSize(hammersleyPairs.GetHammersleyPairs().size(), sizeof(hammersleyPairs.GetHammersleyPairs()[0]));
		hammersleyPairsGraphicsBuffer->SetData(hammersleyPairs.GetHammersleyPairs().data());
	}

	void Graphics::SetupRenderPipelineAssets()
	{
		// horizontalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("HorizontalGaussianCompute");
		// verticalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("VerticalGaussianCompute");
		//
		// gBufferShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBuffer");
		// shadowMapShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("ShadowMap");
		// globalLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferGlobalLighting");
		// localLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferLocalLighting");
		//
		// sphereModel = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");
	}

	void Graphics::Render()
	{
		horizontalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("HorizontalGaussianCompute");
		verticalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("VerticalGaussianCompute");

		gBufferShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBuffer");
		shadowMapShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("ShadowMap");
		globalLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferGlobalLighting");
		localLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferLocalLighting");
		skySphereShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("SkySphere");
		ambientObscurranceShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("AmbientObscurrance");
		ambientObscurranceBilateralBlurShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("AoBilateralBlur");
		bloomBrightColorExtractionShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("BloomBrightColorExtraction");
		bloomPostProcessingShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("BloomPostProcessing");
		
		sphereModel = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");
		
		ITexture2D* hdrSkybox = AssetManagerLocator::Get()->GetTextureRepository().GetAsset("Alexs_Apt_2k.hdr");
		ITexture2D* hdrIrradianceMap = AssetManagerLocator::Get()->GetTextureRepository().GetAsset("Alexs_Apt_2k_Irradiance.hdr");

		LightSystem* lightSystem = LightSystemLocator::Get();
		lightSystem->SetupLightBuffers();


		if (Input::IsKeyPressed(KeyCode::Up))
		{
			roughness += 0.1f;
		}
		if (Input::IsKeyPressed(KeyCode::Down))
		{
			roughness -= 0.1f;
		}

		if (Input::IsKeyPressed(KeyCode::Left))
		{
			metallic -= 0.1f;
		}
		if (Input::IsKeyPressed(KeyCode::Right))
		{
			metallic += 0.1f;
		}

		if (Input::IsKeyPressed(KeyCode::Z))
		{
			exposure -= 0.1f;
		}
		if (Input::IsKeyPressed(KeyCode::X))
		{
			exposure += 0.1f;
		}

		if (Input::IsKeyPressed(KeyCode::C))
		{
			contrast -= 0.1f;
		}
		if (Input::IsKeyPressed(KeyCode::V))
		{
			contrast += 0.1f;
		}

		if (Input::IsKeyPressed(KeyCode::N))
		{
			ambientObscurranceScale -= 0.1f;
		}
		if (Input::IsKeyPressed(KeyCode::M))
		{
			ambientObscurranceScale += 0.1f;
		}

		if (Input::IsKeyPressed(KeyCode::N))
		{
			ambientObscurranceScale -= 1.0f;
		}
		if (Input::IsKeyPressed(KeyCode::M))
		{
			ambientObscurranceScale += 1.0f;
		}

		if (Input::IsKeyPressed(KeyCode::Comma))
		{
			ambientObscurranceContrast -= 1.0f;
		}
		if (Input::IsKeyPressed(KeyCode::Period))
		{
			ambientObscurranceContrast += 1.0f;
		}

		if (Input::IsKeyPressed(KeyCode::K))
		{
			ambientObscurranceWorldSpaceRange -= 1.0f;
		}
		if (Input::IsKeyPressed(KeyCode::L))
		{
			ambientObscurranceWorldSpaceRange += 1.0f;
		}

		if (Input::IsKeyPressed(KeyCode::H))
		{
			ambientObscurranceNumberOfSamples -= 5;
		}
		if (Input::IsKeyPressed(KeyCode::J))
		{
			ambientObscurranceNumberOfSamples += 5;
		}

		if (Input::IsKeyPressed(KeyCode::G))
		{
			ambientObscurranceEnabled = !ambientObscurranceEnabled;
		}

		// ********** Clear dedicated render framebuffer ********** //
		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		renderedSceneRenderTexture->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();
		renderedSceneRenderTexture->Unbind();

		// ********** G-buffer pass ********** //
		const std::vector<MeshRenderer*>& meshRenderers = MeshRendererSystemLocator::Get()->GetMeshRenderers();
		const Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();
		const Mat4F viewMatrix = Camera::GetMainCamera()->GetViewMatrix();

		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
		graphicsContext->CullFaceMode(FaceType::Back);
		graphicsContext->ColorBlendFunction(BlendFunctionType::SourceAlpha, BlendFunctionType::OneMinusSourceAlpha);
		graphicsContext->SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		deferredGBuffer->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();

		gBufferShader->Bind();

		gBufferShader->SetUniformMat4F(viewMatrix, "viewMatrix");
		gBufferShader->SetUniformMat4F(viewProjectionMatrix, "viewProjectionMatrix");
		for (const auto& meshRenderer : meshRenderers)
		{
			gBufferShader->SetUniformVec4F(meshRenderer->GetMaterial()->GetVec4F("diffuseColor"), "diffuseColor");

			const Mat4F modelMatrix = meshRenderer->gameObject->GetTransform()->GetWorldMatrix();
			gBufferShader->SetUniformMat4F(modelMatrix, "modelMatrix");

			meshRenderer->mesh->Bind();
			for (const auto& subMesh : meshRenderer->mesh->GetSubMeshes())
			{
				graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			}
			meshRenderer->mesh->Unbind();
		}

		gBufferShader->Unbind();

		deferredGBuffer->Unbind();

		// Get Attachments for next rendering steps
		ITexture2D* positionAttachment = deferredGBuffer->GetColorAttachment(0);
		ITexture2D* normalAttachment = deferredGBuffer->GetColorAttachment(1);
		ITexture2D* colorAttachment = deferredGBuffer->GetColorAttachment(2);
		ITexture2D* viewSpacePositionAttachment = deferredGBuffer->GetColorAttachment(3);
		ITexture2D* viewSpaceNormalAttachment = deferredGBuffer->GetColorAttachment(4);

		// ********* Ambient Obscurrance Pass ********* //
		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		ambientObscurranceRenderTexture->Bind();

		ambientObscurranceShader->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();

		ambientObscurranceShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "screenWidth");
		ambientObscurranceShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "screenHeight");
		ambientObscurranceShader->SetUniformFloat(ambientObscurranceWorldSpaceRange, "worldSpaceRange");
		ambientObscurranceShader->SetUniformFloat(ambientObscurranceScale, "scale");
		ambientObscurranceShader->SetUniformFloat(ambientObscurranceContrast, "contrast");
		ambientObscurranceShader->SetUniformInt(ambientObscurranceNumberOfSamples, "numberOfSamples");
		ambientObscurranceShader->SetUniformInt(ambientObscurranceEnabled, "enableAmbientObscurrance");
		ambientObscurranceShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");

		ambientObscurranceShader->SetUniformInt(0, "gBufferViewSpacePosition");
		viewSpacePositionAttachment->Bind(0);
		ambientObscurranceShader->SetUniformInt(1, "gBufferViewSpaceNormal");
		viewSpaceNormalAttachment->Bind(1);

		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();

		viewSpacePositionAttachment->Unbind();
		viewSpaceNormalAttachment->Unbind();

		ambientObscurranceShader->Unbind();

		ambientObscurranceRenderTexture->Unbind();

		// ********* Ambient Obscurrance Bilateral Pass ********* //
		ITexture2D* ambientObscurranceAttachment = ambientObscurranceRenderTexture->GetColorAttachment(0);
		
		ambientObscurranceBlurredRenderTexture->Bind();
		ambientObscurranceBilateralBlurShader->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();
		
		ambientObscurranceBilateralBlurShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "screenWidth");
		ambientObscurranceBilateralBlurShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "screenHeight");
		ambientObscurranceBilateralBlurShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");
		
		ambientObscurranceBilateralBlurShader->SetUniformInt(0, "gPosition");
		viewSpacePositionAttachment->Bind(0);
		ambientObscurranceBilateralBlurShader->SetUniformInt(1, "gNormal");
		viewSpaceNormalAttachment->Bind(1);
		ambientObscurranceBilateralBlurShader->SetUniformInt(2, "ambientObscurrance");
		ambientObscurranceAttachment->Bind(2);
		
		ambientObscurranceBilateralBlurShader->SetUniformInt(1, "isHorizonalBlur");
		
		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();
		
		ambientObscurranceBlurredRenderTexture->Unbind();
		ambientObscurranceAttachment->Unbind();
		
		ambientObscurranceRenderTexture->Bind();
		
		ambientObscurranceBilateralBlurShader->SetUniformInt(2, "ambientObscurrance");
		ambientObscurranceBlurredRenderTexture->GetColorAttachment(0)->Bind(2);
		
		ambientObscurranceBilateralBlurShader->SetUniformInt(0, "isHorizonalBlur");
		
		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();
		
		viewSpacePositionAttachment->Unbind();
		viewSpaceNormalAttachment->Unbind();
		ambientObscurranceBlurredRenderTexture->GetColorAttachment(0)->Unbind();
		
		ambientObscurranceRenderTexture->Unbind();
		ambientObscurranceBlurredRenderTexture->Unbind();

		// ********* Global Light Shadow and Lighting Pass ********* //
		Light* globalLight = Light::GetGlobalLight();
		
		if (globalLight->GetShadows() != LightShadows::None)
		{
			// ********* Global Light Shadow Pass ********* //
			graphicsContext->SetViewPort(0, 0, globalLight->GetShadowResolution(), globalLight->GetShadowResolution());

			graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, false);
			graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f } );

			globalLight->GetShadowFrameBuffer()->Bind();

			shadowMapShader->Bind();
			graphicsContext->ClearColor();
			graphicsContext->ClearBuffer();

			const Mat4F shadowViewProjectionMatrix = globalLight->GetShadowViewProjectionMatrix();
			shadowMapShader->SetUniformMat4F(shadowViewProjectionMatrix, "shadowViewProjectionMatrix");
		
			for (const auto& meshRenderer : meshRenderers)
			{
				const Mat4F modelMatrix = meshRenderer->gameObject->GetTransform()->GetWorldMatrix();
				shadowMapShader->SetUniformMat4F(modelMatrix, "modelMatrix");
		
				meshRenderer->mesh->Bind();
				for (const auto& subMesh : meshRenderer->mesh->GetSubMeshes())
				{
					graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
				}
				meshRenderer->mesh->Unbind();
			}
		
			shadowMapShader->Unbind();
		
			globalLight->GetShadowFrameBuffer()->Unbind();

			graphicsContext->SetViewPort(0, 0, WindowApi::GetWindow()->GetWidth(), WindowApi::GetWindow()->GetHeight());

			// ********* Gaussian Compute shader blur step ********* //
			ITexture2D* blurredShadowDepthAttachment = globalLight->GetShadowFrameBuffer()->GetColorAttachment(0);
			ITexture2D* shadowDepthAttachment = globalLight->GetShadowFrameBuffer()->GetColorAttachment(1);

			blurredShadowDepthAttachment->SetBorderColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			blurredShadowDepthAttachment->SetWrapModeU(TextureWrapMode::ClampToBorder);
			blurredShadowDepthAttachment->SetWrapModeV(TextureWrapMode::ClampToBorder);

			// Horizontal blur step
			horizontalComputeShader->Bind();
			
			blurredShadowDepthAttachment->BindAsImageUnit(0, AccessType::Read);
			shadowDepthAttachment->BindAsImageUnit(1, AccessType::Write);
			
			horizontalComputeShader->Dispatch((blurredShadowDepthAttachment->GetWidth() + 128) / 128, blurredShadowDepthAttachment->GetHeight(), 1);
			
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			blurredShadowDepthAttachment->UnbindImageUnit();
			shadowDepthAttachment->UnbindImageUnit();

			horizontalComputeShader->Unbind();

			// Vertical blur step
			verticalComputeShader->Bind();
			
			shadowDepthAttachment->BindAsImageUnit(0, AccessType::Read);
			blurredShadowDepthAttachment->BindAsImageUnit(1, AccessType::Write);
			
			verticalComputeShader->Dispatch(shadowDepthAttachment->GetWidth(), (shadowDepthAttachment->GetHeight() + 128) / 128, 1);
			
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			shadowDepthAttachment->UnbindImageUnit();
			blurredShadowDepthAttachment->UnbindImageUnit();
			
			verticalComputeShader->Unbind();

			// ********* Global Light Lighting Pass ********* //
			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, true);

			renderedSceneRenderTexture->Bind();

			globalLightShader->Bind();
		
			lightSystem->GetGlobalLightBuffer()->Bind(0);
			hammersleyPairsGraphicsBuffer->Bind(3);

			globalLightShader->SetUniformMat4F(shadowViewProjectionMatrix, "shadowViewProjectionMatrix");

			globalLightShader->SetUniformFloat(hdrSkybox->GetWidth(), "screenWidth");
			globalLightShader->SetUniformFloat(hdrSkybox->GetHeight(), "screenHeight");

			globalLightShader->SetUniformFloat(roughness, "roughness");
			globalLightShader->SetUniformFloat(metallic, "metallic");
			globalLightShader->SetUniformFloat(exposure, "exposure");
			globalLightShader->SetUniformFloat(contrast, "contrast");
			globalLightShader->SetUniformFloat(1.0f, "ambientOcclusion");
		
			globalLightShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");
		
			globalLightShader->SetUniformInt(0, "gPosition");
			positionAttachment->Bind(0);
			globalLightShader->SetUniformInt(1, "gNormal");
			normalAttachment->Bind(1);
			globalLightShader->SetUniformInt(2, "gAlbedoSpec");
			colorAttachment->Bind(2);
			globalLightShader->SetUniformInt(3, "shadowMap");
			blurredShadowDepthAttachment->Bind(3);
			globalLightShader->SetUniformInt(4, "hdrIrradianceMap");
			hdrIrradianceMap->Bind(4);
			globalLightShader->SetUniformInt(5, "hdrSkybox");
			hdrSkybox->Bind(5);
			globalLightShader->SetUniformInt(6, "blurredAmbientObscurrance");
			ambientObscurranceRenderTexture->GetColorAttachment(0)->Bind(6);

			quadMesh->Bind();
			graphicsContext->DrawIndexed(DrawType::Triangles, 6);
			quadMesh->Unbind();
		
			positionAttachment->Unbind();
			normalAttachment->Unbind();
			colorAttachment->Unbind();
			ambientObscurranceRenderTexture->GetColorAttachment(0)->Unbind();
			hdrIrradianceMap->Unbind();
			hdrSkybox->Unbind();
		
			lightSystem->GetGlobalLightBuffer()->Unbind();
			hammersleyPairsGraphicsBuffer->Unbind();

			globalLightShader->Unbind();

			renderedSceneRenderTexture->Unbind();

			glBlitNamedFramebuffer(deferredGBuffer->GetFrameBufferHandle(), renderedSceneRenderTexture->GetFrameBufferHandle(), 0, 0, WindowApi::GetWindow()->GetWidth(), WindowApi::GetWindow()->GetHeight(), 0, 0, WindowApi::GetWindow()->GetWidth(), WindowApi::GetWindow()->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}

		
		// ********* Local Lighting Pass ********* //
		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, false);
		graphicsContext->CullFaceMode(FaceType::Front);
		graphicsContext->ColorBlendFunction(BlendFunctionType::One, BlendFunctionType::One);

		renderedSceneRenderTexture->Bind();

		localLightShader->Bind();
		lightSystem->GetPointLightBuffer()->Bind(0);
		sphereModel->renderer->mesh->Bind();
		
		localLightShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "width");
		localLightShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "height");
		
		localLightShader->SetUniformFloat(roughness, "roughness");
		localLightShader->SetUniformFloat(metallic, "metallic");
		localLightShader->SetUniformFloat(exposure, "exposure");
		localLightShader->SetUniformFloat(contrast, "contrast");
		localLightShader->SetUniformFloat(1.0f, "ambientOcclusion");
		
		localLightShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");
		
		localLightShader->SetUniformInt(0, "gPosition");
		positionAttachment->Bind(0);
		localLightShader->SetUniformInt(1, "gNormal");
		normalAttachment->Bind(1);
		localLightShader->SetUniformInt(2, "gAlbedoSpec");
		colorAttachment->Bind(2);
		
		gBufferShader->SetUniformMat4F(viewProjectionMatrix, "viewProjectionMatrix");
		int lightIndex = 0;
		for (auto& light : lightSystem->GetLights())
		{
			if (light == Light::GetGlobalLight())
			{
				continue;
			}
		
			light->GetGameObject().GetTransform()->SetLocalScale(Vec3F( light->GetRange()));
			Mat4F modelMatrix = light->GetGameObject().GetTransform()->GetWorldMatrix();
			localLightShader->SetUniformMat4F(modelMatrix, "modelMatrix");
			localLightShader->SetUniformInt(lightIndex, "lightIndex");
		
			for (const auto& subMesh : sphereModel->renderer->mesh->GetSubMeshes())
			{
				graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			}
			++lightIndex;
		}
		
		positionAttachment->Unbind();
		normalAttachment->Unbind();
		colorAttachment->Unbind();
		
		sphereModel->renderer->mesh->Unbind();
		lightSystem->GetPointLightBuffer()->Unbind();

		localLightShader->Unbind();

		renderedSceneRenderTexture->Unbind();

		// ********* Sky Sphere Pass ********* //

		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
		graphicsContext->CullFaceMode(FaceType::Front);
		graphicsContext->ColorBlendFunction(BlendFunctionType::SourceAlpha, BlendFunctionType::OneMinusSourceAlpha);

		renderedSceneRenderTexture->Bind();

		const Mat4F skySphereModelMatrix = Mat4F::MakeScale(1000.0f);
		skySphereShader->Bind();

		skySphereShader->SetUniformMat4F(viewProjectionMatrix, "viewProjectionMatrix");
		skySphereShader->SetUniformMat4F(skySphereModelMatrix, "modelMatrix");
		skySphereShader->SetUniformFloat(exposure, "exposure");
		skySphereShader->SetUniformFloat(contrast, "contrast");
		skySphereShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");

		hdrSkybox->Bind(0);
		sphereModel->renderer->GetMesh()->Bind();

		for (const auto& subMesh : sphereModel->renderer->mesh->GetSubMeshes())
		{
			graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
		}

		sphereModel->renderer->GetMesh()->Unbind();
		hdrSkybox->Unbind();

		skySphereShader->Unbind();

		renderedSceneRenderTexture->Unbind();

		graphicsContext->CullFaceMode(FaceType::Back);

		// ********* Bloom Pass: Extract Bright Pixels ********* //
		ITexture2D* renderedSceneTexture = renderedSceneRenderTexture->GetColorAttachment(0);

		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		bloomFrameBufferRenderTexture->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();

		bloomBrightColorExtractionShader->Bind();

		bloomBrightColorExtractionShader->SetUniformInt(0, "finalRenderedScene");
		renderedSceneTexture->Bind(0);

		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();

		renderedSceneTexture->Unbind();

		bloomBrightColorExtractionShader->Unbind();

		bloomFrameBufferRenderTexture->Unbind();

		// ********* Bloom Pass: Blur Bright Pixels ********* //
		ITexture2D* bloomBlurredColorAttachment = bloomFrameBufferRenderTexture->GetColorAttachment(0);
		ITexture2D* bloomColorAttachment = bloomFrameBufferRenderTexture->GetColorAttachment(1);

		bloomColorAttachment->SetBorderColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		bloomColorAttachment->SetWrapModeU(TextureWrapMode::ClampToBorder);
		bloomColorAttachment->SetWrapModeV(TextureWrapMode::ClampToBorder);

		// Horizontal blur step
		horizontalComputeShader->Bind();

		bloomBlurredColorAttachment->BindAsImageUnit(0, AccessType::Read);
		bloomColorAttachment->BindAsImageUnit(1, AccessType::Write);

		horizontalComputeShader->Dispatch((WindowApi::GetWindow()->GetWidth() + 128) / 128, WindowApi::GetWindow()->GetHeight(), 1);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		bloomBlurredColorAttachment->UnbindImageUnit();
		bloomColorAttachment->UnbindImageUnit();

		horizontalComputeShader->Unbind();

		// Vertical blur step
		verticalComputeShader->Bind();

		bloomColorAttachment->BindAsImageUnit(0, AccessType::Read);
		bloomBlurredColorAttachment->BindAsImageUnit(1, AccessType::Write);

		verticalComputeShader->Dispatch(WindowApi::GetWindow()->GetWidth(), (WindowApi::GetWindow()->GetHeight() + 128) / 128, 1);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		bloomColorAttachment->UnbindImageUnit();
		bloomBlurredColorAttachment->UnbindImageUnit();

		verticalComputeShader->Unbind();

		// ********* Final Render ********* //
		bloomPostProcessingShader->Bind();

		bloomPostProcessingShader->SetUniformInt(0, "finalRenderedScene");
		renderedSceneTexture->Bind(0);
		bloomPostProcessingShader->SetUniformInt(1, "blurredBloomTexture");
		bloomBlurredColorAttachment->Bind(1);

		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();

		renderedSceneTexture->Unbind();
		bloomBlurredColorAttachment->Unbind();

		bloomPostProcessingShader->Unbind();
	}

	void Graphics::Shutdown()
	{
		deferredGBuffer.reset();
		quadMesh.reset();
		hammersleyPairsGraphicsBuffer.reset();
		ambientObscurranceRenderTexture.reset();
		ambientObscurranceBlurredRenderTexture.reset();
		renderedSceneRenderTexture.reset();
		bloomFrameBufferRenderTexture.reset();
	}
}
