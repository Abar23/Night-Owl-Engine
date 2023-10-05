#pragma once

#include "Scene.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <map>
#include <memory>
#include <string>
#include <type_traits>

namespace NightOwl
{
	class SceneManager
	{
	public:
		SceneManager();

		template <typename T>
		void AddScene()
		{
			ENGINE_ASSERT((std::is_base_of_v<Scene, T>), "Can't add type {0} to Scene Manager", typeid(T).name());

			scenesMap[scenesMap.size()] = std::make_shared<T>();
		}

		Scene* GetActiveScene();

		Scene* GetSceneAt(int sceneId);

		Scene* GetSceneByName(const std::string& name);

		void SetNextSceneToLoad(int sceneIndex);

		void SetNextSceneToLoad(const std::string& sceneName);

		int GetSceneCount();

		bool ShouldLoadNextScene();

		void LoadNextScene();

		void Update();

		void Shutdown();

	private:
		std::map<int, std::shared_ptr<Scene>> scenesMap;

		Scene* activeScene;

		int nextSceneToLoad;

		friend class NightOwlEngine;

		void DestroyMarkedGameObjects();
	};
}
