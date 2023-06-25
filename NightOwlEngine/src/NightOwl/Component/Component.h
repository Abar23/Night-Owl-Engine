#pragma once

#include "Types/ComponentType.h"
#include <memory>

namespace NightOwl
{
	class GameObject;
}

namespace NightOwl
{
	class Component
	{
	public:
		Component(const Component& other) = default;

		Component()
			:	gameObject(nullptr),
				type(ComponentType::None),
				isEnabled(true)
		{ }

		Component(ComponentType type)
			:	gameObject(nullptr),
				type(type),
				isEnabled(true)
		{ }

		virtual ~Component() = default;

		virtual std::shared_ptr<Component> Clone() { return nullptr; }

		GameObject& GetGameObject()
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
		GameObject* gameObject;

		ComponentType type;

		bool isEnabled;

		friend class GameObject;

		virtual void Remove() = 0;
	};
}
