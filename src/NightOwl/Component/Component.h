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

		Component(Component&& other) noexcept
			: gameObject(other.gameObject),
			  type(other.type)
		{
		}

		Component& operator=(const Component& other)
		{
			if (this == &other)
				return *this;
			gameObject = other.gameObject;
			type = other.type;
			return *this;
		}

		Component& operator=(Component&& other) noexcept
		{
			if (this == &other)
				return *this;
			gameObject = other.gameObject;
			type = other.type;
			return *this;
		}

		Component()
			: gameObject(nullptr), type(ComponentType::None)
		{
			
		}

		Component(ComponentType type)
			: gameObject(nullptr), type(type)
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

	protected:
		GameObject::GameObject* gameObject;

		ComponentType type;

		friend class GameObject::GameObject;

	};
}
