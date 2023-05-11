#include <NightOwlPch.h>

#include "NightOwl/Graphics/Materials/StandardMaterial.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"

namespace NightOwl::Graphics
{
	class MeshRenderer;

	StandardMaterial::StandardMaterial()
		:	alpha(1.0f)
	{
		Core::AssetManager* assetManager = Core::AssetManagerLocator::GetAssetManager();
		standardShader = assetManager->LoadShader("Standard Shader", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");
	}

	std::shared_ptr<IMaterial> StandardMaterial::Clone()
	{
		std::shared_ptr<StandardMaterial> clone = std::make_shared<StandardMaterial>();

		clone->standardShader = standardShader;
		clone->diffuseTexture = diffuseTexture;

		return clone;
	}

	void StandardMaterial::Draw(Component::MeshRenderer& meshRenderer)
	{
		auto mesh = meshRenderer.GetMesh();
		auto transform = meshRenderer.GetGameObject().GetTransform();
		standardShader->Bind();
		standardShader->SetUniformMat4F(transform->GetWorldMatrix(), "modelMatrix");
		standardShader->SetUniformMat4F(Component::Camera::GetMainCamera()->GetViewProjectionMatrix(), "viewProjectionMatrix");

		standardShader->SetUniformFloat(alpha, "alpha");

		if(diffuseTexture != nullptr)
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
		RenderApi::GetContext()->DrawIndexed(DrawType::Triangles, mesh->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		mesh->Unbind();

		if(diffuseTexture != nullptr)
		{
			standardShader->SetUniformInt(0, "isInputTextureSet");
			diffuseTexture->Unbind();
		}
		standardShader->Unbind();
	}

	void StandardMaterial::Draw(Physics::Collider2D& collider)
	{
	}

	void StandardMaterial::SetAlpha(float alpha)
	{
		this->alpha = alpha;
	}

	void StandardMaterial::SetTexture(Core::WeakPointer<ITexture2D> texture2D)
	{
		this->diffuseTexture = texture2D;
	}

	Core::WeakPointer<ITexture2D> StandardMaterial::GetTexture()
	{
		return diffuseTexture;
	}
}
