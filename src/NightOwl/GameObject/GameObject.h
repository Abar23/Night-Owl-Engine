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
			if(this->active != active)
			{
				this->active = active;

				active ? OnEnable() : OnDisable();
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

		template<typename T>
		T* AddComponent()
		{
			ENGINE_ASSERT(CheckForComponent<T>() == -1, std::format("Game object {0} already has component {1}", name, typeid(T).name()));

			componentList.push_back(std::make_unique<T>(*this));

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

			ENGINE_ASSERT(componentIndex >= 0, std::format("Removing non-existant component {1} from game object object {0}", name, typeid(T).name()));

			componentList.erase(componentList.begin() + componentIndex);
		}

		template <typename T>
		bool HasComponent() const
		{
			return CheckForComponent<T>() >= 0;
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
	};
}
