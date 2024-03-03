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
		deferredGBuffer.reset(reinterpret_cast<IRenderTexture*>(new OpenGlRenderTexture(window->GetHeight(), 
																						window->GetWidth(),
																						TextureFilterMode::Point, 
																						GraphicsFormat::RGBA32F,
																						GraphicsFormat::Depth24)));
		// add normal color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);
		// add color + specular color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA32F);

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
		// localLightSphere = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");
	}

	void Graphics::Render()
	{
		horizontalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("HorizontalGaussianCompute");
		verticalComputeShader = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("VerticalGaussianCompute");

		gBufferShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBuffer");
		shadowMapShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("ShadowMap");
		globalLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferGlobalLighting");
		localLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferLocalLighting");

		localLightSphere = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");


		LightSystem* lightSystem = LightSystemLocator::Get();
		lightSystem->SetupLightBuffers();

		// ********** G-buffer pass ********** //
		const std::vector<MeshRenderer*>& meshRenderers = MeshRendererSystemLocator::Get()->GetMeshRenderers();
		const Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();

		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
		graphicsContext->CullFaceMode(FaceType::Back);
		graphicsContext->ColorBlendFunction(BlendFunctionType::SourceAlpha, BlendFunctionType::OneMinusSourceAlpha);
		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		deferredGBuffer->Bind();
		graphicsContext->ClearColor();
		graphicsContext->ClearBuffer();

		gBufferShader->Bind();

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

		deferredGBuffer->Unbind();

		// Get Attachments for next rendering steps
		ITexture2D* positionAttachment = deferredGBuffer->GetColorAttachment(0);
		ITexture2D* normalAttachment = deferredGBuffer->GetColorAttachment(1);
		ITexture2D* colorAttachment = deferredGBuffer->GetColorAttachment(2);

		// ********* Global Light Shadow and Lighting Pass ********* //
		Light* globalLight = Light::GetGlobalLight();
		
		if (globalLight->GetShadows() != LightShadows::None)
		{
			// ********* Global Light Shadow Pass ********* //
			graphicsContext->SetViewPort(0, 0, globalLight->GetShadowResolution(), globalLight->GetShadowResolution());

			graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, false);
			graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });

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

			globalLightShader->Bind();
		
			lightSystem->GetGlobalLightBuffer()->Bind(0);
		
			globalLightShader->SetUniformMat4F(shadowViewProjectionMatrix, "shadowViewProjectionMatrix");
		
			globalLightShader->SetUniformFloat(0.5f, "roughness");
			globalLightShader->SetUniformFloat(0.0f, "metallic");
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
		
			quadMesh->Bind();
			graphicsContext->DrawIndexed(DrawType::Triangles, 6);
			quadMesh->Unbind();
		
			positionAttachment->Unbind();
			normalAttachment->Unbind();
			colorAttachment->Unbind();
			blurredShadowDepthAttachment->Unbind();
		
			lightSystem->GetGlobalLightBuffer()->Unbind();
		
			globalLightShader->Unbind();
		}

		
		// ********* Local Lighting Pass ********* //
		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, false);
		graphicsContext->CullFaceMode(FaceType::Front);
		graphicsContext->ColorBlendFunction(BlendFunctionType::One, BlendFunctionType::One);
		
		localLightShader->Bind();
		lightSystem->GetPointLightBuffer()->Bind(0);
		localLightSphere->renderer->mesh->Bind();
		
		localLightShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "width");
		localLightShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "height");
		
		localLightShader->SetUniformFloat(0.5f, "roughness");
		localLightShader->SetUniformFloat(0.0f, "metallic");
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
		
			light->GetGameObject().GetTransform()->SetLocalScale(light->GetRange());
			Mat4F modelMatrix = light->GetGameObject().GetTransform()->GetWorldMatrix();
			localLightShader->SetUniformMat4F(modelMatrix, "modelMatrix");
			localLightShader->SetUniformInt(lightIndex, "lightIndex");
		
			for (const auto& subMesh : localLightSphere->renderer->mesh->GetSubMeshes())
			{
				graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			}
			++lightIndex;
		}
		
		positionAttachment->Unbind();
		normalAttachment->Unbind();
		colorAttachment->Unbind();
		
		localLightSphere->renderer->mesh->Unbind();
		lightSystem->GetPointLightBuffer()->Unbind();
		localLightShader->Unbind();
	}

	void Graphics::Shutdown()
	{
		deferredGBuffer.reset();
		quadMesh.reset();
	}
}
