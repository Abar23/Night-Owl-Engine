#pragma once

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include <vector>
#include <set>

namespace NightOwl::Graphics
{
	class MeshRendererSystem
	{
	public:
		void Update();

		void AddMeshRenderer(Component::MeshRenderer* meshRenderer);

		void RemoveMeshRenderer(const Component::MeshRenderer* meshRenderer);

	private:
		std::vector<Component::MeshRenderer*> meshRenderers;
	};
}

