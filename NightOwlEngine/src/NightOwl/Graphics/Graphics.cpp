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

	void Graphics::Render()
	{
		LightSystem* lightSystem = LightSystemLocator::Get();
		lightSystem->SetupLightBuffers();

		// ********** G-buffer pass ********** //
		const IShader* gBufferShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBuffer");
		const std::vector<MeshRenderer*>& meshRenderers = MeshRendererSystemLocator::Get()->GetMeshRenderers();
		const Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();

		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
		graphicsContext->CullFaceMode(FaceType::Back);
		graphicsContext->ColorBlendFunction(BlendFunctionType::SourceAlpha, BlendFunctionType::OneMinusSourceAlpha);
		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		deferredGBuffer->Bind();
		graphicsContext->ClearBuffer();
		graphicsContext->ClearColor();

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
		const IShader* shadowMapShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("ShadowMap");
		
		if (globalLight->GetShadows() != LightShadows::None)
		{
			// ********* Global Light Shadow Pass ********* //
			glViewport(0, 0, globalLight->GetShadowResolution(), globalLight->GetShadowResolution());

			//graphicsContext->CullFaceMode(FaceType::Front);
			graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, true);
			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, true);
			graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

			globalLight->GetShadowFrameBuffer()->Bind();

			shadowMapShader->Bind();
			graphicsContext->ClearBuffer();
			graphicsContext->ClearColor();
			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, false);

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
		
			//graphicsContext->CullFaceMode(FaceType::Back);

			glViewport(0, 0, WindowApi::GetWindow()->GetWidth(), WindowApi::GetWindow()->GetHeight());

			// // ********* Gaussian Compute shader blur step ********* //
			const IComputeShader* horizontalCompute = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("HorizontalGaussianCompute");
			const IComputeShader* verticalCompute = AssetManagerLocator::Get()->GetComputeShaderRepository().GetAsset("VerticalGaussianCompute");
			ITexture2D* blurredShadowDepthAttachment = globalLight->GetShadowFrameBuffer()->GetColorAttachment(0);
			ITexture2D* shadowDepthAttachment = globalLight->GetShadowFrameBuffer()->GetColorAttachment(1);
			
			horizontalCompute->Bind();
			
			blurredShadowDepthAttachment->BindAsImageUnit(0, AccessType::Read);
			shadowDepthAttachment->BindAsImageUnit(1, AccessType::Write);
			
			horizontalCompute->Dispatch((blurredShadowDepthAttachment->GetWidth() + 128) / 128, blurredShadowDepthAttachment->GetHeight(), 1);
			
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			blurredShadowDepthAttachment->UnbindImageUnit();
			shadowDepthAttachment->UnbindImageUnit();
			
			horizontalCompute->Unbind();
			
			verticalCompute->Bind();
			
			shadowDepthAttachment->BindAsImageUnit(0, AccessType::Read);
			blurredShadowDepthAttachment->BindAsImageUnit(1, AccessType::Write);
			
			verticalCompute->Dispatch(shadowDepthAttachment->GetWidth(), (shadowDepthAttachment->GetHeight() + 128) / 128, 1);
			
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
			shadowDepthAttachment->UnbindImageUnit();
			blurredShadowDepthAttachment->UnbindImageUnit();
			
			verticalCompute->Unbind();
		
			// ********* Global Light Lighting Pass ********* //
			const IShader* gBufferLightingShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferGlobalLighting");

			graphicsContext->EnableCapability(ContextCapabilityType::ColorBlend, true);

			gBufferLightingShader->Bind();
		
			lightSystem->GetGlobalLightBuffer()->Bind(0);
		
			gBufferLightingShader->SetUniformMat4F(shadowViewProjectionMatrix, "shadowViewProjectionMatrix");
		
			gBufferLightingShader->SetUniformFloat(0.5f, "roughness");
			gBufferLightingShader->SetUniformFloat(0.0f, "metallic");
			gBufferLightingShader->SetUniformFloat(1.0f, "ambientOcclusion");
		
			gBufferLightingShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");
		
			gBufferLightingShader->SetUniformInt(0, "gPosition");
			positionAttachment->Bind(0);
			gBufferLightingShader->SetUniformInt(1, "gNormal");
			normalAttachment->Bind(1);
			gBufferLightingShader->SetUniformInt(2, "gAlbedoSpec");
			colorAttachment->Bind(2);
			gBufferLightingShader->SetUniformInt(3, "shadowMap");
			blurredShadowDepthAttachment->Bind(3);
		
			quadMesh->Bind();
			graphicsContext->DrawIndexed(DrawType::Triangles, 6);
			quadMesh->Unbind();
		
			positionAttachment->Unbind();
			normalAttachment->Unbind();
			colorAttachment->Unbind();
			blurredShadowDepthAttachment->Unbind();
		
			lightSystem->GetGlobalLightBuffer()->Unbind();
		
			gBufferLightingShader->Unbind();
		}

		
		// ********* Local Lighting Pass ********* //
		const Model* sphere = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");
		const IShader* debugLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferLocalLighting");
		
		graphicsContext->EnableCapability(ContextCapabilityType::DepthTest, false);
		graphicsContext->CullFaceMode(FaceType::Front);
		graphicsContext->ColorBlendFunction(BlendFunctionType::One, BlendFunctionType::One);
		graphicsContext->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		
		debugLightShader->Bind();
		lightSystem->GetPointLightBuffer()->Bind(0);
		sphere->renderer->mesh->Bind();
		
		debugLightShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "width");
		debugLightShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "height");
		
		debugLightShader->SetUniformFloat(0.5f, "roughness");
		debugLightShader->SetUniformFloat(0.0f, "metallic");
		debugLightShader->SetUniformFloat(1.0f, "ambientOcclusion");
		
		debugLightShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");
		
		debugLightShader->SetUniformInt(0, "gPosition");
		positionAttachment->Bind(0);
		debugLightShader->SetUniformInt(1, "gNormal");
		normalAttachment->Bind(1);
		debugLightShader->SetUniformInt(2, "gAlbedoSpec");
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
			debugLightShader->SetUniformMat4F(modelMatrix, "modelMatrix");
			debugLightShader->SetUniformInt(lightIndex, "lightIndex");
		
			for (const auto& subMesh : sphere->renderer->mesh->GetSubMeshes())
			{
				graphicsContext->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			}
			++lightIndex;
		}
		
		positionAttachment->Unbind();
		normalAttachment->Unbind();
		colorAttachment->Unbind();
		
		sphere->renderer->mesh->Unbind();
		lightSystem->GetPointLightBuffer()->Unbind();
		debugLightShader->Unbind();
	}

	void Graphics::Shutdown()
	{
		deferredGBuffer.reset();
		quadMesh.reset();
	}
}
