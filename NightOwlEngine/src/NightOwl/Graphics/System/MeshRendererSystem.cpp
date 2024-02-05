#include "NightOwlPch.h"

#include "MeshRendererSystem.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	void MeshRendererSystem::Update()
	{
		std::sort(meshRenderers.begin(), meshRenderers.end(), Comparator);

		for (auto meshRenderer : meshRenderers)
		{
			if(meshRenderer->GetGameObject().IsActive())
			{
				meshRenderer->Draw();
			}
		}
	}

	void MeshRendererSystem::AddMeshRenderer(MeshRenderer* meshRenderer)
	{
		meshRenderers.push_back(meshRenderer);
	}

	void MeshRendererSystem::RemoveMeshRenderer(const MeshRenderer* meshRenderer)
	{
		int meshRendererIndex = 0;
		for (const auto renderer : meshRenderers)
		{
			if (renderer == meshRenderer)
			{
				break;
			}
			meshRendererIndex++;
		}

		if (meshRendererIndex != meshRenderers.size())
		{
			meshRenderers.erase(meshRenderers.begin() + meshRendererIndex);
		}
	}

	const std::vector<MeshRenderer*>& MeshRendererSystem::GetMeshRenderers()
	{
		return meshRenderers;
	}

	bool MeshRendererSystem::Comparator(MeshRenderer* lhs,
	                                    MeshRenderer* rhs)
	{
		const Vec3F cameraPosition = Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition();

		const float lhsSquareDistance = Vec3F(cameraPosition - lhs->GetGameObject().GetTransform()->GetPosition()).SquareMagnitude();
		const float rhsSquareDistance = Vec3F(cameraPosition - rhs->GetGameObject().GetTransform()->GetPosition()).SquareMagnitude();

		return rhsSquareDistance < lhsSquareDistance;
	}
}
