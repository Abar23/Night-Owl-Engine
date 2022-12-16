#pragma once

#include "NightOwl/Graphics/System/MeshRendererSystem.h"

namespace NightOwl::Core
{
	class MeshRendererSystemLocator
	{
	public:
		static Graphics::MeshRendererSystem* GetMeshRendererSystem()
		{
			return meshRendererSystemInstance;
		}

		static void Provide(Graphics::MeshRendererSystem* meshRendererSystem)
		{
			meshRendererSystemInstance = meshRendererSystem;
		}

	private:
		inline static Graphics::MeshRendererSystem* meshRendererSystemInstance{ nullptr };
	};
}
