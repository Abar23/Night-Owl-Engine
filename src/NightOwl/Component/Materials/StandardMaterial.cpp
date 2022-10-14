#include "NightOwl/Component/Materials/StandardMaterial.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"

namespace NightOwl::Component
{
	class MeshRenderer;

	StandardMaterial::StandardMaterial()
	{
		standardShader = Graphics::RenderApi::CreateShader("Standard Shader", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");
	}

	void StandardMaterial::Draw(MeshRenderer& meshRenderer)
	{
		auto mesh = meshRenderer.GetMesh();
		auto transform = meshRenderer.GetGameObject().GetTransform();
		standardShader->Bind();
		standardShader->SetUniformMat4F(transform->GetWorldMatrix(), "modelMatrix");
		standardShader->SetUniformMat4F(Camera::GetMainCamera()->ViewProjectionMatrix(), "viewProjectionMatrix");

		if(diffuseTexture)
		{
			standardShader->SetUniformInt(0, "inputTexture");
			standardShader->SetUniformInt(1, "isInputTextureSet");
			diffuseTexture->Bind(0);
		}
		else
		{
			standardShader->SetUniformInt(0, "isInputTextureSet");
		}

		mesh->Bind();
		Graphics::RenderApi::GetContext()->DrawIndexed(Graphics::DrawType::Triangles, mesh->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		mesh->Unbind();

		if(diffuseTexture)
		{
			standardShader->SetUniformInt(0, "isInputTextureSet");
			diffuseTexture->Unbind();
		}
		standardShader->Unbind();
	}

	void StandardMaterial::SetTexture(std::shared_ptr<Graphics::ITexture2D> texture2D)
	{
		this->diffuseTexture = texture2D;
	}
}
