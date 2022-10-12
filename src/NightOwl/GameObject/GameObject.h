#pragma once

#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Behavior/IBehavior.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace NightOwl::Component
{
	class Component;
}

namespace NightOwl::GameObject
{
	class GameObject : private Behavior::IBehavior
	{
	public:
		GameObject()
			: transform(*this), active(true)
		{
		}

		GameObject(const std::string& name)
			: transform(*this), name(name), active(true)
		{
		}

		bool IsEnabled() const
		{
			return active;
		}

		void SetActive(bool active)
		{
			this->active = active;

			active ? OnEnable() : OnDisable() ;
		}

		const std::string& GetName()
		{
			return name;
		}

		void SetName(const std::string& name)
		{
			this->name = name;
		}

		Component::Transform& GetTransform()
		{
			return transform;
		}

		template<typename T>
		void AddComponent()
		{
			ENGINE_ASSERT(!CheckForComponent<T>(), std::format("Game object {0} already has component {1}", name, typeid(T).name()));

			componentList.push_back(std::make_unique<T>(*this));
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
			ENGINE_ASSERT(CheckForComponent<T>(), std::format("Removing non-existant component {1} from game object object {0}", name, typeid(T).name()));

			int index = 0;
			for (const auto& component : componentList)
			{
				if (dynamic_cast<T*>(component.get()))
				{
					break;
				}
				index++;
			}

			componentList.erase(componentList.begin() + index);
		}

		void OnAwake() override;

		void OnUpdate() override;

		void OnEnable() override;

		void OnDisable() override;

	protected:
		std::vector<std::unique_ptr<Component::Component>> componentList;

		Component::Transform transform;

		std::string name;

		bool active;

		template <typename T>
		bool CheckForComponent() const
		{
			if (std::is_same_v<T, Component::Transform>) return true;

			for(const auto& component : componentList)
			{
				if (dynamic_cast<T*>(component.get())) return true;
			}
			return false;
		}
	};
}
