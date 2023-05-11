#pragma once

#include "Scene.h"
#include <map>
#include <string>
#include <type_traits>
#include <memory>

namespace NightOwl::Core
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

		WeakPointer<Scene> GetActiveScene();

		WeakPointer<Scene> GetSceneAt(int sceneId);

		WeakPointer<Scene> GetSceneByName(const std::string& name);

		void SetNextSceneToLoad(int sceneIndex);

		void SetNextSceneToLoad(const std::string& sceneName);

		int GetSceneCount();

		bool ShouldLoadNextScene();

		void LoadNextScene();

		void Update();

		void Shutdown();

	private:
		std::map<int, std::shared_ptr<Scene>> scenesMap;

		WeakPointer<Scene> activeScene;

		int nextSceneToLoad;

		friend class NightOwlEngine;

		void DestroyMarkedGameObjects();
	};
}
