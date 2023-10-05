#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

namespace NightOwl
{
	class GameObject;

	class SceneManager;

	class SceneSerializer;

	class Transform;

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

		GameObject& AddGameObject();

		GameObject& AddGameObject(const std::string& name);

		GameObject& AddCloneOfGameObject(GameObject& gameObject);

		//GameObject& AddArchetype(const std::string& archetypeName, const std::string& name);

		void RemoveGameObject(const GameObject& gameObject);

		std::vector<GameObject*>& GetRootGameObjects();

		GameObject* FindWithName(const std::string& gameObjectName);

		std::vector<GameObject*> FindGameObjectsWithName(const std::string& gameObjectName);

		GameObject* FindWithTag(const std::string& gameObjectTag);

		std::vector<GameObject*> FindGameObjectsWithTag(const std::string& gameObjectTag);

		std::string GetName();

	protected:
		std::map<unsigned int, std::shared_ptr<GameObject>> gameObjectsList;

		std::vector<GameObject*> rootGameObjects;

		std::string name;

		bool isDirty;

		inline static unsigned int idCounter = 0;

		friend class SceneSerializer;

		friend class Transform;

		friend class SceneManager;

		friend class GameObject;

		void SetDirtyFlag();

		void ReleaseResources();

		void DestroyMarkedGameObjects();

		void AddClone(const std::shared_ptr<GameObject>& clone);
	};
}

