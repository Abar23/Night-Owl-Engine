#pragma once

#include "Types/ComponentType.h"

namespace NightOwl::GameObjects
{
	class GameObject;
}


namespace NightOwl::Components
{
	class Component
	{
	public:
		Component() = delete;

		Component(GameObjects::GameObject* gameObject, ComponentType type)
			: gameObject(gameObject), type(type)
		{
			
		}

		virtual ~Component() = default;

		GameObjects::GameObject* GetGameObject() const
		{
			return gameObject;
		}

		void SetGameObject(GameObjects::GameObject* gameObject)
		{
			this->gameObject = gameObject;
		}

		ComponentType GetComponentType()
		{
			return type;
		}

	private:
		GameObjects::GameObject* gameObject;

		ComponentType type;
	};
}
