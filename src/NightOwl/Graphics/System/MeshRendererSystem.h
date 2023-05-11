#pragma once

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include <vector>

namespace NightOwl::Graphics
{
	class MeshRendererSystem
	{
	public:
		void Update();

		void AddMeshRenderer(const Component::MeshRenderer* meshRenderer);

		void RemoveMeshRenderer(const Component::MeshRenderer* meshRenderer);

	private:
		std::vector<Component::MeshRenderer*> meshRenderers;

		static bool Comparator(const Component::MeshRenderer* lhs, const Component::MeshRenderer* rhs);
	};
}

