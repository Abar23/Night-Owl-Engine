#pragma once

#include "NightOwl/GameObject/GameObject.h"
#include <map>
#include <vector>

namespace NightOwl::Core
{
	class SceneSerializer;

	class Scene
	{
	public:
		Scene();

		void Update();

		GameObject::GameObject& AddGameObject();

		GameObject::GameObject& AddGameObject(const std::string& name);

		void RemoveGameObject(const GameObject::GameObject& gameObject);

		std::vector<GameObject::GameObject*>& GetRootGameObjects();

	private:
		friend class SceneSerializer;

		std::map<unsigned int, std::shared_ptr<GameObject::GameObject>> gameObjectsList;

		std::vector<GameObject::GameObject*> rootGameObjects;

		bool isDirty;

		inline static unsigned int idCounter = 0;

		void SetDirtyFlag();
	};
}

