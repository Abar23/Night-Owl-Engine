#include <NightOwlPch.h>

#include "Scene.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Core/Asset/AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	Scene::Scene(std::string sceneName)
		:	name(std::move(sceneName)),
			isDirty(false)
	{ }

	void Scene::Update()
	{
		const auto& updatedRootGameObjects = GetRootGameObjects();

		for(GameObject* parent : updatedRootGameObjects)
		{
			if (parent->IsActive())
			{
				for (int childIndex = 0; childIndex < parent->GetTransform()->GetNumberOfChildren(); childIndex++)
				{
					Transform* childTransform = parent->GetTransform()->GetChildAtIndex(childIndex);
					childTransform->PropagateParentTransform(parent->GetTransform()->localVecQuatMat);
				}
			}
		}
	}

	GameObject& Scene::AddGameObject()
	{
		auto& gameObject = gameObjectsList.insert({ idCounter, std::make_shared<GameObject>(this, idCounter) }).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject;
	}

	GameObject& Scene::AddGameObject(const std::string& name)
	{
		auto& gameObject = gameObjectsList.insert({idCounter, std::make_shared<GameObject>(name, this, idCounter)}).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject;
	}

	GameObject& Scene::AddCloneOfGameObject(GameObject& gameObject)
	{
		std::shared_ptr<GameObject> clone = gameObject.Clone(this);
		SetDirtyFlag();
		return *clone;
	}

	void Scene::RemoveGameObject(const GameObject& gameObject)
	{
		unsigned int id = gameObject.GetId();
		
		ENGINE_ASSERT(gameObjectsList.contains(id), "Trying to remove game object with id {0} that is not in the scene", id);

		gameObjectsList.erase(id);

		SetDirtyFlag();
	}

	std::vector<GameObject*>& Scene::GetRootGameObjects()
	{
		if (isDirty)
		{
			rootGameObjects.clear();
			for (const auto& pair : gameObjectsList)
			{
				const Transform* transform = pair.second->GetTransform();
				if (!transform->HasParent())
				{
					rootGameObjects.push_back(pair.second.get());
				}
			}
			isDirty = false;
		}

		return rootGameObjects;
	}

	GameObject* Scene::FindWithName(const std::string& gameObjectName)
	{
		for (const auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			if(gameObjectPtr->GetName() == gameObjectName)
			{
				return gameObjectPtr.get();
			}
		}

		return nullptr;
	}

	std::vector<GameObject*> Scene::FindGameObjectsWithName(const std::string& gameObjectName)
	{
		std::vector<GameObject*> gameObjects;
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			if (gameObjectPtr->GetName() == gameObjectName)
			{
				gameObjects.push_back(gameObjectPtr.get());
			}
		}
		return gameObjects;
	}

	GameObject* Scene::FindWithTag(const std::string& gameObjectTag)
	{
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			//if (tagComponent->GetTag() == gameObjectTag)
			//{
			//	return gameObjectPtr.get();
			//}
		}

		return nullptr;
	}

	std::vector<GameObject*> Scene::FindGameObjectsWithTag(const std::string& gameObjectTag)
	{
		std::vector<GameObject*> gameObjects;
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			//if (tagComponent->GetTag() == gameObjectTag)
			//{
			//	gameObjects.push_back(gameObjectPtr.get());
			//}
		}

		return gameObjects;
	}

	std::string Scene::GetName()
	{
		return name;
	}

	void Scene::SetDirtyFlag()
	{
		isDirty = true;
	}

	void Scene::ReleaseResources()
	{
		// Remove resources loaded in asset manager
		AssetManager* assetManager = AssetManagerLocator::Get();
		assetManager->ClearSceneAll();

		// Destroy all game objects in the scene
		const auto& updatedRootGameObjects = GetRootGameObjects();
		for (const auto& rootGameObject : updatedRootGameObjects)
		{
			rootGameObject->DestroyGameObject();
		}

		OwlBehaviorManagerLocator::Get()->Reset();

		rootGameObjects.clear();
	}

	void Scene::DestroyMarkedGameObjects()
	{
		// Need to get updated list of game objects since some may have been destroyed last frame
		const auto& updatedRootGameObjects = GetRootGameObjects();

		for(GameObject* gameObject : updatedRootGameObjects)
		{
			if (gameObject->shouldDestroy)
			{
				gameObject->DestroyGameObject();
			}
		}
	}

	void Scene::AddClone(const std::shared_ptr<GameObject>& clone)
	{
		clone->id = idCounter;
		gameObjectsList.emplace(idCounter, clone);
		idCounter++;
	}
}
