#pragma once

#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include <memory>
#include <string>
#include <vector>

namespace NightOwl::Core
{
	class Scene;
	class SceneSerializer;
}

namespace NightOwl::Component
{
	class Component;
}

namespace NightOwl::Physics
{
	class PhysicsEngine2D;
}

namespace NightOwl::GameObject
{
	class GameObject
	{
	public:
		GameObject(Core::Scene* scene, unsigned int id)
			: scene(scene),
		      id(id),
		      isActive(true)
		{
			transform.gameObject = this;
		}

		GameObject(const std::string& name, Core::Scene* scene, unsigned int id)
			: scene(scene),
		      name(name),
		      id(id),
		      isActive(true)
		{
			transform.gameObject = this;
		}


		bool IsActive() const
		{
			return isActive;
		}

		void SetActive(bool active)
		{
			if(this->isActive != active)
			{
				this->isActive = active;

				for (auto& element : componentList)
				{
					element.get()->isEnabled = this->isActive;
				}
			}
		}

		const std::string& GetName()
		{
			return name;
		}

		void SetName(const std::string& name)
		{
			this->name = name;
		}

		Component::Transform* GetTransform()
		{
			return &transform;
		}

		unsigned int GetId() const
		{
			return id;
		}

		template<typename T>
		T* AddComponent()
		{
			ENGINE_ASSERT(CheckForComponent<T>() == -1, "Game object {0} already has component {1}", name, typeid(T).name());

			std::shared_ptr<Component::Component> component = std::make_shared<T>();

			componentList.push_back(component);

			component->gameObject = this;

			// Check if new component is an owl behavior. If so, add to the owl behavior manager
			Behavior::OwlBehavior* owlBehavior = dynamic_cast<Behavior::OwlBehavior*>(component.get());

			if (owlBehavior != nullptr)
			{
				Core::OwlBehaviorManagerLocator::GetOwlBehaviorManager()->AddOwlBehavior(owlBehavior);
				owlBehaviorList.insert(owlBehavior);
			}

			return dynamic_cast<T*>(componentList.back().get());
		}

		template <typename T>
		T* GetComponent() const
		{
			for (const auto& component : componentList)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					return dynamic_cast<T*>(component.get());
				}
			}

			return nullptr;
		}

		template <typename T>
		void RemoveComponent()
		{
			const int componentIndex = CheckForComponent<T>();

			ENGINE_ASSERT(componentIndex >= 0, "Removing non-existant component {1} from game object object {0}", name, typeid(T).name());

			// Check if component being removed is an owl behavior. If so, remove from the owl behavior manager
			Behavior::OwlBehavior* owlBehavior = dynamic_cast<Behavior::OwlBehavior*>(componentList[componentIndex].get());

			if (owlBehavior != nullptr)
			{
				Core::OwlBehaviorManagerLocator::GetOwlBehaviorManager()->RemoveOwlBehavior(owlBehavior);
				owlBehaviorList.erase(owlBehavior);
			}

			componentList.erase(componentList.begin() + componentIndex);
		}

		template <typename T>
		bool HasComponent() const
		{
			return CheckForComponent<T>() >= 0;
		}

	protected:
		std::vector<std::shared_ptr<Component::Component>> componentList;

		std::set<Behavior::OwlBehavior*> owlBehaviorList;

		Component::Transform transform;

		Core::Scene* scene;

		std::string name;

		unsigned int id;

		bool isActive;

		friend class Physics::PhysicsEngine2D;

		friend class Core::SceneSerializer;

		template <typename T>
		int CheckForComponent() const
		{
			int notFound = -1;

			if (std::is_same_v<T, Component::Transform>) return notFound;

			for (int componentIndex = 0; componentIndex < componentList.size(); componentIndex++)
			{
				if (dynamic_cast<T*>(componentList[componentIndex].get())) return componentIndex;
			}

			return notFound;
		}

		const std::set<Behavior::OwlBehavior*>& GetOwlBehaviorList()
		{
			return owlBehaviorList;
		}
	};
}
