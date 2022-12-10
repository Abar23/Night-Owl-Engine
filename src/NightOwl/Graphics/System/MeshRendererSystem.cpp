#include "MeshRendererSystem.h"

#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Graphics
{
	void MeshRendererSystem::Update()
	{
		for (auto* meshRenderer : meshRenderers)
		{
			if(meshRenderer->GetGameObject().IsActive())
			{
				meshRenderer->Draw();
			}
		}
	}

	void MeshRendererSystem::AddMeshRenderer(Component::MeshRenderer* meshRenderer)
	{
		meshRenderers.push_back(meshRenderer);
	}

	void MeshRendererSystem::RemoveMeshRenderer(const Component::MeshRenderer* meshRenderer)
	{
		int meshRendererIndex = 0;
		for (const auto* renderer : meshRenderers)
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
}
