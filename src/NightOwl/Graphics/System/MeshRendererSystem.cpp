#include "NightOwlPch.h"

#include "MeshRendererSystem.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Graphics
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

	void MeshRendererSystem::AddMeshRenderer(const Component::MeshRenderer* meshRenderer)
	{
		meshRenderers.push_back(meshRenderer);
	}

	void MeshRendererSystem::RemoveMeshRenderer(const Component::MeshRenderer* meshRenderer)
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

	bool MeshRendererSystem::Comparator(const Component::MeshRenderer* lhs,
										const Component::MeshRenderer* rhs)
	{
		const Math::Vec3F cameraPosition = Component::Camera::GetMainCamera()->GetGameObject().GetTransform()->GetPosition();

		const float lhsSquareDistance = Math::Vec3F(cameraPosition - lhs->GetGameObject().GetTransform()->GetPosition()).SquareMagnitude();
		const float rhsSquareDistance = Math::Vec3F(cameraPosition - rhs->GetGameObject().GetTransform()->GetPosition()).SquareMagnitude();

		return rhsSquareDistance < lhsSquareDistance;
	}
}
