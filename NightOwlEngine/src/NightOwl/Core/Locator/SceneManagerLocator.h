#pragma once

#include "NightOwl/Core/Application/SceneManager.h"

namespace NightOwl
{
	class SceneManagerLocator
	{
	public:
		static SceneManager* GetSceneManager()
		{
			return sceneManagerInstance;
		}

		static void Provide(SceneManager* sceneManager)
		{
			sceneManagerInstance = sceneManager;
		}

	private:
		inline static SceneManager* sceneManagerInstance{ nullptr };
	};
}
