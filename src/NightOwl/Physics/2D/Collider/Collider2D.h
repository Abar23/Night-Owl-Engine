#pragma once

#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl::Component
{
	class RigidBody2D;
}

namespace NightOwl::Physics
{
	class Collider2D
	{
	public:
		Collider2D()
			:	rigidBody2D(nullptr)
		{ }

		virtual ~Collider2D() = default;

		Component::RigidBody2D* GetRigidBody()
		{
			return rigidBody2D;
		}

		REFLECT()

	protected:
		Component::RigidBody2D* rigidBody2D;

		friend class Component::RigidBody2D;
	};
}
