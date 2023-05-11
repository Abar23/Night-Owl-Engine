#pragma once

#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Core/Memory/WeakPointer.h"
#include <map>
#include <vector>
#include <string>

namespace NightOwl::Component
{
	class Transform;
}

namespace NightOwl::Core
{
	class SceneManager;
}

namespace NightOwl::Core
{
	class SceneSerializer;

	class Scene
	{
	public:
		Scene(std::string sceneName);

		virtual ~Scene() = default;

		void Update();

		virtual void Init() = 0;

		virtual void Reset() = 0;

		virtual void Load() = 0;

		virtual void Shutdown() = 0;

		GameObject::GameObject& AddGameObject();

		GameObject::GameObject& AddGameObject(const std::string& name);

		GameObject::GameObject& AddArchetype(const std::string& archetypeName, const std::string& name);

		void RemoveGameObject(const GameObject::GameObject& gameObject);

		std::vector<WeakPointer<GameObject::GameObject>>& GetRootGameObjects();

		WeakPointer<GameObject::GameObject> FindWithName(const std::string& gameObjectName);

		std::vector<WeakPointer<GameObject::GameObject>> FindGameObjectsWithName(const std::string& gameObjectName);

		WeakPointer<GameObject::GameObject> FindWithTag(const std::string& gameObjectTag);

		std::vector<WeakPointer<GameObject::GameObject>> FindGameObjectsWithTag(const std::string& gameObjectTag);

		std::string GetName();

	protected:
		std::map<unsigned int, std::shared_ptr<GameObject::GameObject>> gameObjectsList;

		std::vector<WeakPointer<GameObject::GameObject>> rootGameObjects;

		std::string name;

		bool isDirty;

		inline static unsigned int idCounter = 0;

		friend class SceneSerializer;

		friend class Component::Transform;

		friend class SceneManager;

		friend class GameObject::GameObject;

		void SetDirtyFlag();

		void ReleaseResources();

		void DestroyMarkedGameObjects();

		void AddClone(const std::shared_ptr<GameObject::GameObject>& clone);
	};
}

