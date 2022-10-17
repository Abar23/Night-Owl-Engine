#include "Scene.h"

namespace NightOwl::Core
{
	Scene::Scene()
		: isDirty(false)
	{
	}

	void Scene::Update()
	{
		GetRootGameObjects();

		for(GameObject::GameObject* parent : rootGameObjects)
		{
			for(int childIndex = 0; childIndex < parent->GetTransform()->GetNumberOfChildren(); childIndex++)
			{
				Component::Transform* childTransform = parent->GetTransform()->GetChildAtIndex(childIndex);
				childTransform->PropagateParentLocalTransform(parent->GetTransform()->GetLocalModelMatrix());
			}
		}
	}

	GameObject::GameObject& Scene::AddGameObject()
	{
		auto gameObject = gameObjectsList.insert({ idCounter, std::make_shared<GameObject::GameObject>(this, idCounter) }).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject.get();
	}

	GameObject::GameObject& Scene::AddGameObject(const std::string& name)
	{
		auto gameObject = gameObjectsList.insert({idCounter, std::make_shared<GameObject::GameObject>(name, this, idCounter)}).first->second;
		idCounter++;
		SetDirtyFlag();
		return *gameObject.get();
	}

	void Scene::RemoveGameObject(const GameObject::GameObject& gameObject)
	{
		unsigned int id = gameObject.GetId();
		
		ENGINE_ASSERT(gameObjectsList.contains(id), std::format("Trying to remove game object with id {0} that is not in the scene", id));

		gameObjectsList.erase(id);
	}

	std::vector<GameObject::GameObject*>& Scene::GetRootGameObjects()
	{
		if (isDirty)
		{
			rootGameObjects.clear();
			for (auto& pair : gameObjectsList)
			{
				Component::Transform* transform = pair.second->GetTransform();
				if (!transform->HasParent())
				{
					rootGameObjects.push_back(pair.second.get());
				}
			}
			isDirty = false;
		}

		return rootGameObjects;
	}

	void Scene::SetDirtyFlag()
	{
		isDirty = true;
	}
}
