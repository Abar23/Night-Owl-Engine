#pragma once

#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"
#include "NightOwl/Core/Utitlity/Assert.h"

namespace NightOwl
{
	template<typename T>
	T* GameObject::AddComponent()
	{
		ENGINE_ASSERT(CheckForComponent<T>() == -1, "Game object {0} already has component {1}", name, typeid(T).name());

		std::shared_ptr<Component> component = std::make_shared<T>();

		componentList.push_back(component);

		component->gameObject = this;

		// Check if new component is an owl behavior. If so, add to the owl behavior manager
		OwlBehavior* owlBehavior = dynamic_cast<OwlBehavior*>(component.get());

		if (owlBehavior != nullptr)
		{
			OwlBehaviorManagerLocator::GetOwlBehaviorManager()->AddOwlBehavior(owlBehavior);
			owlBehaviorList.insert(owlBehavior);
		}

		return dynamic_cast<T*>(componentList.back().get());
	}

	template <typename T>
	T* GameObject::GetComponent() const
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
	void GameObject::RemoveComponent()
	{
		ENGINE_ASSERT(typeid(T) != typeid(Transform), "Removing Transform component from gameobject");

		const int componentIndex = CheckForComponent<T>();

		ENGINE_ASSERT(componentIndex >= 0, "Removing non-existant component {1} from game object object {0}", name, typeid(T).name());

		// Check if component being removed is an owl behavior. If so, remove from the owl behavior manager
		Component* component = (componentList[componentIndex].get());

		// Remove component from system
		component->Remove();

		// Remove from owl behavior list if component is owl behavior
		OwlBehavior* owlBehavior = dynamic_cast<OwlBehavior*>(component);
		if (owlBehavior != nullptr)
		{
			owlBehaviorList.erase(owlBehavior);
		}

		componentList.erase(componentList.begin() + componentIndex);
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		return CheckForComponent<T>() >= 0;
	}

	template <typename T>
	int GameObject::CheckForComponent() const
	{
		int notFound = -1;

		if (std::is_same_v<T, Transform>) return notFound;

		for (int componentIndex = 0; componentIndex < componentList.size(); componentIndex++)
		{
			if (dynamic_cast<T*>(componentList[componentIndex].get())) return componentIndex;
		}

		return notFound;
	}
}
