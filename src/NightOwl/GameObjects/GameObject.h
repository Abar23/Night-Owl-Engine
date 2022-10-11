#pragma once

#include "NightOwl/Components/Component.h"
#include "NightOwl/Components/Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <memory>
#include <string>
#include <vector>

#include "NightOwl/Components/NullComponet.h"

namespace NightOwl::GameObjects
{
	class GameObject
	{
	public:
		GameObject(const std::string& name)
			: name(name), active(true)
		{
			componentList.push_back(std::make_shared<Components::Transform>(this));
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

		void AddComponent(const std::shared_ptr<Components::Component>& component)
		{
			ENGINE_ASSERT(gameObjectDoesNotHaveCopmonentType(component->GetComponentType()), 
				std::format("Game object {0} already has component {1}", name, ComponentTypeToString(component->GetComponentType())));

			componentList.push_back(component);
		}

		Components::Component* GetComponentByType(Components::ComponentType type)
		{
			for (auto& component : componentList)
			{
				if (component->GetComponentType() == type) return component.get();
			}
			return nullptr;
		}

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void OnEnable() = 0;

		virtual void OnDisable() = 0;

	private:
		std::vector<std::shared_ptr<Components::Component>> componentList;

		std::string name;

		bool active;

		bool gameObjectDoesNotHaveCopmonentType(Components::ComponentType type)
		{
			for(const auto& componet : componentList)
			{
				if (componet->GetComponentType() == type) return false;
			}
			return true;
		}
	};
}
