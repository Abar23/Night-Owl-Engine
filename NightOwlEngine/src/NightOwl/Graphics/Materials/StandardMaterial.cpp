#include <NightOwlPch.h>

#include "NightOwl/Graphics/Materials/StandardMaterial.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"

namespace NightOwl
{
	StandardMaterial::StandardMaterial()
		:	diffuseTexture(nullptr),
			alpha(1.0f)
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();
		standardShader = assetManager->LoadShader("Standard Shader", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");
	}

	std::shared_ptr<IMaterial> StandardMaterial::Clone()
	{
		std::shared_ptr<StandardMaterial> clone = std::make_shared<StandardMaterial>();

		clone->standardShader = standardShader;
		clone->diffuseTexture = diffuseTexture;

		return clone;
	}

	void StandardMaterial::Draw(Renderer& renderer)
	{
		const std::shared_ptr<Mesh> mesh = renderer.GetMesh();
		Transform* transform = renderer.GetGameObject().GetTransform();

		standardShader->Bind();
		standardShader->SetUniformMat4F(transform->GetWorldMatrix(), "modelMatrix");
		standardShader->SetUniformMat4F(Camera::GetMainCamera()->GetViewProjectionMatrix(), "viewProjectionMatrix");

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
		const std::vector<SubMeshData>& subMeshDatas = mesh->GetSubMeshes();
		if (subMeshDatas.empty())
		{
			RenderApi::GetContext()->DrawIndexed(DrawType::Triangles, mesh->GetNumberOfTriangles() * 3);
		}
		else
		{
			RenderApi::GetContext()->DrawIndexed(DrawType::Triangles, subMeshDatas[0].indexStart);

			for (const SubMeshData& subMeshData : subMeshDatas)
			{
			 	RenderApi::GetContext()->DrawIndexedBaseVertex(DrawType::Triangles, subMeshData.indexCount, subMeshData.indexStart, subMeshData.baseVertex);
			}
		}
		mesh->Unbind();


		if(diffuseTexture != nullptr)
		{
			standardShader->SetUniformInt(0, "isInputTextureSet");
			diffuseTexture->Unbind();
		}
		standardShader->Unbind();
	}

	void StandardMaterial::Draw(Collider2D& collider)
	{
	}

	void StandardMaterial::SetAlpha(float alpha)
	{
		this->alpha = alpha;
	}

	void StandardMaterial::SetTexture(ITexture2D* texture2D)
	{
		this->diffuseTexture = texture2D;
	}

	ITexture2D* StandardMaterial::GetTexture()
	{
		return diffuseTexture;
	}
}
