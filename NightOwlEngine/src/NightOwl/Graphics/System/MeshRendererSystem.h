#pragma once

#include <vector>

namespace NightOwl
{
	class MeshRenderer;

	class MeshRendererSystem
	{
	public:
		void Update();

		void AddMeshRenderer(MeshRenderer* meshRenderer);

		void RemoveMeshRenderer(const MeshRenderer* meshRenderer);

	private:
		std::vector<MeshRenderer*> meshRenderers;

		static bool Comparator(MeshRenderer* lhs, MeshRenderer* rhs);
	};
}
