#pragma once

#include "Types/ComponentType.h"

namespace NightOwl::GameObject
{
	class GameObject;
}

namespace NightOwl::Component
{
	class Component
	{
	public:
		Component(const Component& other) = default;

		Component()
			: gameObject(nullptr), type(ComponentType::None), isEnabled(true)
		{
			
		}

		Component(ComponentType type)
			: gameObject(nullptr), type(type), isEnabled(true)
		{

		}

		virtual ~Component() = default;

		GameObject::GameObject& GetGameObject()
		{
			return *gameObject;
		}

		ComponentType GetComponentType() const
		{
			return type;
		}

		void SetEnabled(bool enabled)
		{
			isEnabled = enabled;
		}

		bool GetEnabled() const
		{
			return isEnabled;
		}

	protected:
		GameObject::GameObject* gameObject;

		ComponentType type;

		bool isEnabled;

		friend class GameObject::GameObject;
	};
}
