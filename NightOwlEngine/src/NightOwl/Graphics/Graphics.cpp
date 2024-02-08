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
		// ********** G-buffer pass ********** //
		const IShader* gBufferShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBuffer");
		const std::vector<MeshRenderer*>& meshRenderers = MeshRendererSystemLocator::Get()->GetMeshRenderers();
		const Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();

		glEnable(GL_DEPTH_TEST);
		GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		deferredGBuffer->Bind();
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

		// // ********* Global Lighting Pass ********* //
		const IShader* gBufferLightingShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferGlobalLighting");
		
		LightSystem* lightSystem = LightSystemLocator::Get();
		
		ITexture2D* positionAttachment = deferredGBuffer->GetColorAttachment(0);
		ITexture2D* normalAttachment = deferredGBuffer->GetColorAttachment(1);
		ITexture2D* colorAttachment = deferredGBuffer->GetColorAttachment(2);
		
		gBufferLightingShader->Bind();
		
		gBufferLightingShader->SetUniformFloat(0.5f, "roughness");
		gBufferLightingShader->SetUniformFloat(0.0f, "metallic");
		gBufferLightingShader->SetUniformFloat(0.0f, "ambientOcclusion");
		
		gBufferLightingShader->SetUniformVec3F(Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition(), "cameraPosition");

		gBufferLightingShader->SetUniformInt(0, "gPosition");
		positionAttachment->Bind(0);
		gBufferLightingShader->SetUniformInt(1, "gNormal");
		normalAttachment->Bind(1);
		gBufferLightingShader->SetUniformInt(2, "gAlbedoSpec");
		colorAttachment->Bind(2);
		
		quadMesh->Bind();
		graphicsContext->DrawIndexed(DrawType::Triangles, 6);
		quadMesh->Unbind();
		
		positionAttachment->Unbind();
		normalAttachment->Unbind();
		colorAttachment->Unbind();

		gBufferLightingShader->Unbind();
		
		// // ********* Copy Depth from G-Buffer to Back Buffer ********* //
		// IWindow* window = WindowApi::GetWindow().get();
		// // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		// // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		// // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		// GL_CALL(glBlitNamedFramebuffer, deferredGBuffer->GetFrameBufferHandle(), 0, 0, 0, window->GetWidth(), window->GetHeight(), 0, 0, window->GetWidth(), window->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		//

		// ********* Local Lighting Pass ********* //
		const Model* sphere = AssetManagerLocator::Get()->GetModelRepository().GetAsset("sphere");
		const IShader* debugLightShader = AssetManagerLocator::Get()->GetShaderRepository().GetAsset("GBufferLocalLighting");

		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_ONE);
		glCullFace(GL_FRONT);

		debugLightShader->Bind();
		lightSystem->SetupLightBuffer();
		lightSystem->GetLightBuffer()->Bind(0);
		sphere->renderer->mesh->Bind();

		debugLightShader->SetUniformFloat(WindowApi::GetWindow()->GetWidth(), "width");
		debugLightShader->SetUniformFloat(WindowApi::GetWindow()->GetHeight(), "height");

		debugLightShader->SetUniformFloat(0.2f, "roughness");
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
		lightSystem->GetLightBuffer()->Unbind();
		debugLightShader->Unbind();
	}

	void Graphics::Shutdown()
	{
		deferredGBuffer.release();
		quadMesh.reset();
	}
}
