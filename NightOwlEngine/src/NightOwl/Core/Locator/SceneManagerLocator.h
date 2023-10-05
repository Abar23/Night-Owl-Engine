#pragma once

namespace NightOwl
{
	class SceneManager;

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
