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
		Component() = delete;

		Component(GameObject::GameObject* gameObject, ComponentType type)
			: gameObject(gameObject), type(type)
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

	};
}
