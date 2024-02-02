#include "NightOwlPch.h"

#include "Renderer.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/Structures/KeyFrames.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Graphics/Graphics.h"
#include <stack>

#include "NightOwl/Graphics/System/LightSystem.h"

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
		const Mat4F modelMatrix = transform->GetWorldMatrix();
		const Mat4F viewProjectionMatrix = Camera::GetMainCamera()->GetViewProjectionMatrix();

		// Calculating final bones here since the scene graph has propagated all of the parent local changes made in the animator component
		// Should be moved to skinned mesh renderer since these components are tightly coupled
		if (gameObject->HasComponent<Animator>())
		{
			if (finalBoneMatrices.empty() == true)
			{
				finalBoneMatrices.resize(100);
			}

			const Animator* animator = gameObject->GetComponent<Animator>();

			auto& boneInfoMap = mesh->GetBoneInfoMap();

			std::stack<Transform*> skeletonTransforms;
			skeletonTransforms.push(animator->GetSkeleton());
			while (skeletonTransforms.empty() == false)
			{
				Transform* skeletonTransform = skeletonTransforms.top();
				skeletonTransforms.pop();

				for (int skeletonTransformChildIndex = 0; skeletonTransformChildIndex < skeletonTransform->GetNumberOfChildren(); ++skeletonTransformChildIndex)
				{
					skeletonTransforms.push(skeletonTransform->GetChildAtIndex(skeletonTransformChildIndex));
				}

				const auto& boneName = skeletonTransform->GetGameObject().GetName();
				if (boneInfoMap.contains(boneName))
				{
					BoneInfo boneInfo = boneInfoMap.at(boneName);
					const Mat4F finalBoneOffsetMatrix = skeletonTransform->GetWorldMatrix() * boneInfo.offsetMatrix;
					finalBoneMatrices[boneInfo.id] = finalBoneOffsetMatrix;
				}
			}
		}

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
				for (int boneIndex = 0; boneIndex < finalBoneMatrices.size(); ++boneIndex)
				{
					if (boneIndex == 0)
					{
						currentMaterial->SetMat4F(finalBoneMatrices[0], "finalBonesMatrices[0]");
					}
					currentMaterial->GetShader()->SetUniformMat4F(finalBoneMatrices[boneIndex], "finalBonesMatrices[" + std::to_string(boneIndex) + "]");
				}
				currentMaterial->SetInteger(1, "hasBones");
			}
			else
			{
				currentMaterial->SetInteger(0, "hasBones");
			}

			LightSystemLocator::Get()->SetupLightBuffer();
			currentMaterial->SetBuffer(LightSystemLocator::Get()->GetLightBuffer().get(), "lightStorageBlock");

			currentMaterial->Bind();
			Graphics::GetContext()->DrawIndexedBaseVertex(DrawType::Triangles, subMesh.indexCount, subMesh.indexStart, subMesh.baseVertex);
			currentMaterial->Unbind();

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

	bool Renderer::IsVisible() const
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
		mesh->isReadable = clone->mesh->isReadable;
		mesh->UploadMeshData(mesh->isReadable);

		materials.resize(clone->materials.size());
		for (int materialIndex = 0; materialIndex < clone->materials.size(); ++materialIndex)
		{
			materials[materialIndex] = clone->materials[materialIndex]->Clone();
		}
	}
}
