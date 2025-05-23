#pragma once

namespace NightOwl
{
	class Collider2D;

	class GameObject;

	class RigidBody2D;

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

		RigidBody2D* GetBody() const
		{
			return body;
		}

		RigidBody2D* GetOtherBody() const
		{
			return otherBody;
		}

		GameObject* GetObject() const
		{
			return object;
		}

	private:
		Collider2D* collider;

		Collider2D* otherCollider;

		RigidBody2D* body;

		RigidBody2D* otherBody;

		GameObject* object;

		friend class PhysicsEngine2D;
	};
}
