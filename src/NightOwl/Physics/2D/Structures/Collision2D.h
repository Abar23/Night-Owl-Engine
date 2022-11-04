#pragma once

#include "NightOwl/Physics/2D/Collider/Collider2D.h"

namespace NightOwl::GameObject
{
	class GameObject;
}

namespace NightOwl::Physics
{
	class Collision2D
	{
	public:
		Collision2D(Collider2D* collider, Collider2D* otherCollider);

		Collider2D* GetCollider() const
		{
			return collider;
		}

		Collider2D* GetOtherCollider() const
		{
			return otherCollider;
		}

		Component::RigidBody2D* GetBody() const
		{
			return body;
		}

		Component::RigidBody2D* GetOtherBody() const
		{
			return otherBody;
		}

		GameObject::GameObject* GetObject() const
		{
			return object;
		}

	private:
		Collider2D* collider;

		Collider2D* otherCollider;

		Component::RigidBody2D* body;

		Component::RigidBody2D* otherBody;

		GameObject::GameObject* object;

		friend class PhysicsEngine2D;
	};
}
