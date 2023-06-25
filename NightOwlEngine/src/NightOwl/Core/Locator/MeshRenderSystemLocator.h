#pragma once

#include "NightOwl/Graphics/System/MeshRendererSystem.h"

namespace NightOwl
{
	class MeshRendererSystemLocator
	{
	public:
		static MeshRendererSystem* GetMeshRendererSystem()
		{
			return meshRendererSystemInstance;
		}

		static void Provide(MeshRendererSystem* meshRendererSystem)
		{
			meshRendererSystemInstance = meshRendererSystem;
		}

	private:
		inline static MeshRendererSystem* meshRendererSystemInstance{ nullptr };
	};
}
