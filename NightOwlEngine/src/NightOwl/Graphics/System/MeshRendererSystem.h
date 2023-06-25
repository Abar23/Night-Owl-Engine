#pragma once

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include <vector>

namespace NightOwl
{
	class MeshRendererSystem
	{
	public:
		void Update();

		void AddMeshRenderer(const MeshRenderer* meshRenderer);

		void RemoveMeshRenderer(const MeshRenderer* meshRenderer);

	private:
		std::vector<MeshRenderer*> meshRenderers;

		static bool Comparator(const MeshRenderer* lhs, const MeshRenderer* rhs);
	};
}

