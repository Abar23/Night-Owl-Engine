#pragma once

#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl::Physics
{
	class Collision2D
	{
	public:
		Collision2D() = delete;

		Collision2D(Component::RigidBody2D* collider, Component::RigidBody2D* otherCollider)
			:	collider(collider),
				otherCollider(otherCollider)
		{ }

	private:
		Component::RigidBody2D* collider;

		Component::RigidBody2D* otherCollider;
	};
}
