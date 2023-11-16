#pragma once

#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Behavior/OwlBehavior.h"
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace NightOwl
{
	class PhysicsEngine2D;

	class Component;

	class Scene;

	class SceneSerializer;

	class GameObject
	{
	public:
		GameObject();

		GameObject(Scene* scene, unsigned int id);

		GameObject(const std::string& name, Scene* scene, unsigned int id);

		bool IsActive() const;

		void SetActive(bool active);

		const std::string& GetName();

		void SetName(const std::string& name);

		void SetScene(Scene* scene);

		Transform* GetTransform();

		unsigned int GetId() const;

		template<typename T>
		T* AddComponent();

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent() const;

		Scene* GetScene();

		std::shared_ptr<GameObject> Clone(Scene* currentScene = nullptr, Transform* parent = nullptr);

		void Destroy();

	protected:
		std::vector<std::shared_ptr<Component>> componentList;

		std::set<OwlBehavior*> owlBehaviorList;

		Transform transform;

		Scene* scene;

		std::string name;

		unsigned int id;

		bool isActive;

		bool shouldDestroy;

		friend class PhysicsEngine2D;

		friend class SceneSerializer;

		friend class Scene;

		template <typename T>
		int CheckForComponent() const;

		const std::set<OwlBehavior*>& GetOwlBehaviorList();

		void DestroyGameObject();
	};
}

#include "GameObject.inl"
