#include <NightOwlPch.h>

#include "Scene.h"

#include "NightOwl/Component/Concrete/Tag.h"
#include "NightOwl/Core/Locator/ArchetypeSystemLocator.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"

namespace NightOwl::Core
{
	Scene::Scene(std::string sceneName)
		:	name(std::move(sceneName)),
			isDirty(false)
	{ }

	void Scene::Update()
	{
		const auto& updatedRootGameObjects = GetRootGameObjects();

		for(WeakPointer<GameObject::GameObject> parent : updatedRootGameObjects)
		{
			if (parent->IsActive())
			{
				Math::Mat4F parentLocalModelMatrix = parent->GetTransform()->GetLocalModelMatrix();
				for (int childIndex = 0; childIndex < parent->GetTransform()->GetNumberOfChildren(); childIndex++)
				{
					WeakPointer<Component::Transform> childTransform = parent->GetTransform()->GetChildAtIndex(childIndex);
					childTransform->PropagateParentLocalTransform(parentLocalModelMatrix);
				}
			}
		}
	}

	GameObject::GameObject& Scene::AddGameObject()
	{
		auto& gameObject = gameObjectsList.insert({ idCounter, std::make_shared<GameObject::GameObject>(this, idCounter) }).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject;
	}

	GameObject::GameObject& Scene::AddGameObject(const std::string& name)
	{
		auto& gameObject = gameObjectsList.insert({idCounter, std::make_shared<GameObject::GameObject>(name, this, idCounter)}).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject;
	}

	GameObject::GameObject& Scene::AddArchetype(const std::string& archetypeName, const std::string& name)
	{
		auto gameObject = ArchetypeSystemLocator::GetArchetypeSystem()->GetArchetype(archetypeName);
		gameObject->name = name;
		gameObject->scene = this;
		AddClone(gameObject);
		SetDirtyFlag();
		return *gameObject;
	}

	void Scene::RemoveGameObject(const GameObject::GameObject& gameObject)
	{
		unsigned int id = gameObject.GetId();
		
		ENGINE_ASSERT(gameObjectsList.contains(id), "Trying to remove game object with id {0} that is not in the scene", id);

		gameObjectsList.erase(id);

		SetDirtyFlag();
	}

	std::vector<WeakPointer<GameObject::GameObject>>& Scene::GetRootGameObjects()
	{
		if (isDirty)
		{
			rootGameObjects.clear();
			for (auto& pair : gameObjectsList)
			{
				const WeakPointer<Component::Transform> transform = pair.second->GetTransform();
				if (!transform->HasParent())
				{
					rootGameObjects.push_back(pair.second.get());
				}
			}
			isDirty = false;
		}

		return rootGameObjects;
	}

	WeakPointer<GameObject::GameObject> Scene::FindWithName(const std::string& gameObjectName)
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

	std::vector<WeakPointer<GameObject::GameObject>> Scene::FindGameObjectsWithName(const std::string& gameObjectName)
	{
		std::vector<WeakPointer<GameObject::GameObject>> gameObjects;
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			if (gameObjectPtr->GetName() == gameObjectName && gameObjectPtr->IsActive())
			{
				gameObjects.push_back(gameObjectPtr.get());
			}
		}
		return gameObjects;
	}

	WeakPointer<GameObject::GameObject> Scene::FindWithTag(const std::string& gameObjectTag)
	{
		for (const auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			auto tagComponent = gameObjectPtr->GetComponent<Component::Tag>();

			if (tagComponent != nullptr && tagComponent->GetTag() == gameObjectTag)
			{
				return gameObjectPtr.get();
			}
		}

		return nullptr;
	}

	std::vector<WeakPointer<GameObject::GameObject>> Scene::FindGameObjectsWithTag(const std::string& gameObjectTag)
	{
		std::vector<WeakPointer<GameObject::GameObject>> gameObjects;
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			auto tagComponent = gameObjectPtr->GetComponent<Component::Tag>();

			if (tagComponent != nullptr && tagComponent->GetTag() == gameObjectTag)
			{
				gameObjects.push_back(gameObjectPtr.get());
			}
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
		auto* assetManager = AssetManagerLocator::GetAssetManager();
		assetManager->ClearAll();

		// Remove any archetypes loaded
		auto* archetypeSystem = ArchetypeSystemLocator::GetArchetypeSystem();
		archetypeSystem->ReleaseResources();

		// Destroy all game objects in the scene
		const auto& updatedRootGameObjects = GetRootGameObjects();
		for (auto it = updatedRootGameObjects.cbegin(); it != updatedRootGameObjects.cend(); ++it)
		{
			it->GetPointer()->DestroyGameObject();
		}
		rootGameObjects.clear();
	}

	void Scene::DestroyMarkedGameObjects()
	{
		// Need to get updated list of game objects since some may have been destroyed last frame
		const auto& updatedRootGameObjects = GetRootGameObjects();

		for(const WeakPointer<GameObject::GameObject>& gameObject : updatedRootGameObjects)		{
			if (gameObject->shouldDestroy)
			{
				gameObject->DestroyGameObject();
			}
		}
	}

	void Scene::AddClone(const std::shared_ptr<GameObject::GameObject>& clone)
	{
		clone->id = idCounter;
		gameObjectsList.insert({ idCounter, clone });
		idCounter++;
	}
}
