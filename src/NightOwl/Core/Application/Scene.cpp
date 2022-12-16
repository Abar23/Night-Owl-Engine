#include "Scene.h"

#include <ranges>

namespace NightOwl::Core
{
	Scene::Scene()
		: isDirty(false)
	{ }

	void Scene::Update()
	{
		GetRootGameObjects();

		for(GameObject::GameObject* parent : rootGameObjects)
		{
			if (parent->IsActive())
			{
				Math::Mat4F parentLocalModelMatrix = parent->GetTransform()->GetLocalModelMatrix();
				for (int childIndex = 0; childIndex < parent->GetTransform()->GetNumberOfChildren(); childIndex++)
				{
					Component::Transform* childTransform = parent->GetTransform()->GetChildAtIndex(childIndex);
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

	void Scene::RemoveGameObject(const GameObject::GameObject& gameObject)
	{
		unsigned int id = gameObject.GetId();
		
		ENGINE_ASSERT(gameObjectsList.contains(id), "Trying to remove game object with id {0} that is not in the scene", id);

		gameObjectsList.erase(id);
	}

	std::vector<GameObject::GameObject*>& Scene::GetRootGameObjects()
	{
		if (isDirty)
		{
			rootGameObjects.clear();
			for (auto& pair : gameObjectsList)
			{
				const Component::Transform* transform = pair.second->GetTransform();
				if (!transform->HasParent())
				{
					rootGameObjects.push_back(pair.second.get());
				}
			}
			isDirty = false;
		}

		return rootGameObjects;
	}

	GameObject::GameObject* Scene::FindGameObject(const std::string& gameObjectName)
	{
		for (auto& gameObjectPtr : gameObjectsList | std::views::values)
		{
			std::shared_ptr<GameObject::GameObject> object = gameObjectPtr;

			if(gameObjectPtr->GetName() == gameObjectName)
			{
				return gameObjectPtr.get();
			}
		}

		return nullptr;
	}

	void Scene::SetDirtyFlag()
	{
		isDirty = true;
	}
}
