#include "NightOwlPch.h"

#include "Renderer.h"

#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Graphics/Materials/Material.h"

namespace NightOwl
{
	Renderer::Renderer()
		: isVisible(true)
	{
		mesh = std::make_shared<Mesh>();
	}

	void Renderer::Draw()
	{
		if (isVisible == false)
		{
			return;
		}

		Transform* transform = gameObject->GetTransform();
		Mat4F modelMatrix = transform->GetWorldMatrix();
		Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();

		mesh->Bind();
		for (const auto& subMesh : mesh->GetSubMeshes())
		{
			const auto& currentMaterial = materials[subMesh.materialId];

			currentMaterial->GetShader()->Bind();

			currentMaterial->SetMat4F(modelMatrix, "modelMatrix");
			currentMaterial->SetMat4F(viewProjectionMatrix, "viewProjectionMatrix");

			currentMaterial->SetInteger(0, "isInputTextureSet");

			if (finalBoneMatrices.empty() == false)
			{
				for (int i = 0; i < finalBoneMatrices.size(); ++i)
				{
					if (i == 0)
					{
						currentMaterial->SetMat4F(finalBoneMatrices[0], "finalBonesMatrices[0]");
					}
					currentMaterial->GetShader()->SetUniformMat4F(finalBoneMatrices[i], "finalBonesMatrices[" + std::to_string(i) + "]");
				}
				currentMaterial->SetInteger(1, "hasBones");
			}
			else
			{
				currentMaterial->SetInteger(0, "hasBones");
			}

			currentMaterial->Bind();
			RenderApi::GetContext()->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			currentMaterial->UnBind();

			currentMaterial->GetShader()->Unbind();
		}
		mesh->Unbind();
	}

	std::shared_ptr<Mesh> Renderer::GetMesh()
	{
		return mesh;
	}

	void Renderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	void Renderer::SetVisible(bool isVisible)
	{
		this->isVisible = isVisible;
	}

	bool Renderer::IsVisible()
	{
		return isVisible;
	}

	const std::shared_ptr<Material> Renderer::GetMaterial()
	{
		return materials[0];
	}

	void Renderer::SetMaterial(const std::shared_ptr<Material>& material)
	{
		this->materials[0] = material->Clone();
	}

	const std::vector<std::shared_ptr<Material>>& Renderer::GetMaterials()
	{
		return materials;
	}

	void Renderer::SetMaterials(const std::vector<std::shared_ptr<Material>>& materials)
	{
		this->materials.resize(materials.size());
		for (const auto& material : materials)
		{
			this->materials[0] = material->Clone();
		}
	}

	std::vector<Mat4F>& Renderer::GetFinalBoneMatrices()
	{
		return finalBoneMatrices;
	}

	void Renderer::CloneRenderer(const std::shared_ptr<Renderer>& clone)
	{
		mesh->SetUVs(clone->mesh->GetUVs());
		mesh->SetVertices(clone->mesh->GetVertices());
		mesh->SetColors(clone->mesh->GetColors());
		mesh->SetBitangents(clone->mesh->GetBitangents());
		mesh->SetTangents(clone->mesh->GetTangents());
		mesh->SetNormals(clone->mesh->GetNormals());
		mesh->SetBoneWeights(clone->mesh->GetBoneWeights());
		mesh->SetTriangles(clone->mesh->GetTriangles());
		mesh->SetSubMeshes(clone->mesh->GetSubMeshes());
		mesh->SetBoneInfoMap(clone->mesh->GetBoneInfoMap());
		isVisible = clone->isVisible;

		materials.resize(clone->materials.size());
		for (int materialIndex = 0; materialIndex < clone->materials.size(); ++materialIndex)
		{
			materials[materialIndex] = clone->materials[materialIndex]->Clone();
		}
	}
}
