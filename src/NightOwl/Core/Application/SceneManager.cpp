#include "NightOwlPch.h"

#include "SceneManager.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include "NightOwl/Core/Locator/TileSystemLocator.h"

namespace NightOwl::Core
{
	SceneManager::SceneManager()
		:	activeScene(nullptr),
			nextSceneToLoad(-1)
	{ }

	WeakPointer<Scene> SceneManager::GetActiveScene()
	{
		return activeScene;
	}

	WeakPointer<Scene> SceneManager::GetSceneAt(int sceneId)
	{
		ENGINE_ASSERT(scenesMap.contains(sceneId), "Scene Manager does not contain scene at index {0}", sceneId);

		return scenesMap[sceneId].get();
	}

	WeakPointer<Scene> SceneManager::GetSceneByName(const std::string& name)
	{
		WeakPointer<Scene> scene = nullptr;

		for(const auto& sceneInMap : scenesMap | std::views::values)
		{
			if(sceneInMap->GetName() == name)
			{
				scene = sceneInMap.get();
				break;
			}
		}

		ENGINE_ASSERT(scene != nullptr, "Scene Manager does not contain scene with name {0}", name);

		return scene;
	}

	void SceneManager::SetNextSceneToLoad(const int sceneIndex)
	{
		ENGINE_ASSERT(scenesMap.contains(sceneIndex), "Scene Manager does not contain scene at index {0}", sceneIndex);

		ENGINE_ASSERT(sceneIndex >= 0, "Invalid scene index to load give to Scene Manager. Index provided was {0}", sceneIndex);

		nextSceneToLoad = sceneIndex;
	}

	void SceneManager::SetNextSceneToLoad(const std::string& sceneName)
	{
		for (auto& [sceneIndex, sceneInMap] : scenesMap)
		{
			if (sceneInMap->GetName() == sceneName)
			{
				nextSceneToLoad = sceneIndex;
				break;
			}
		}

		ENGINE_ASSERT(nextSceneToLoad != -1, "Scene Manager does not contain scene with name {0}", sceneName);
	}

	int SceneManager::GetSceneCount()
	{
		return scenesMap.size();
	}

	bool SceneManager::ShouldLoadNextScene()
	{
		return nextSceneToLoad != -1;
	}

	void SceneManager::LoadNextScene()
	{
		if(activeScene != nullptr)
		{
			activeScene->ReleaseResources();

			activeScene->Shutdown();
		}

		activeScene = scenesMap.at(nextSceneToLoad).get();

		activeScene->Load();

		activeScene->Init();

		activeScene->Update();

		OwlBehaviorManagerLocator::GetOwlBehaviorManager()->InitBehaviors();

		nextSceneToLoad = -1;
	}

	void SceneManager::Update()
	{
		ENGINE_ASSERT(activeScene != nullptr, "Scene Manager active scene is null on update");

		activeScene->Update();
	}

	void SceneManager::Shutdown()
	{
		activeScene->ReleaseResources();

		activeScene = nullptr;
	}

	void SceneManager::DestroyMarkedGameObjects()
	{
		activeScene->DestroyMarkedGameObjects();
	}
}
