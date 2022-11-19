#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Physics/2D/Collider/Collider2D.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include <memory>
#include <set>

namespace NightOwl::Physics
{
	class PhysicsEngine2D;

	class Collision2D;
}

namespace NightOwl::Component
{
	class RigidBody2D : public Component
	{
	public:
		RigidBody2D();

		~RigidBody2D() override;

		void AddForce(Math::Vec2F forceVector);

		Math::Vec2F GetVelocity() const;

		void SetVelocity(const Math::Vec2F& velocity);

		float GetRotation() const;

		void SetRotation(float rotation);

		float GetMass() const;

		void SetMass(float mass);

		float GetInverseMass() const;

		Math::Vec2F GetPosition() const;

		void SetPosition(const Math::Vec2F& position);

		Math::Vec2F GetAccumulatedForces() const;

		void ClearForces();

		Physics::Collider2D* GetCollider();

		void SetCollider(Physics::Collider2D* collider);

		REFLECT()

	private:
		Math::Vec2F velocity;

		float rotation; // z-axis only

		float mass;

		float inverseMass;

		Math::Vec2F position;

		Math::Vec2F linearVelocity;

		Math::Vec2F accumulatedForces;

		std::shared_ptr<Physics::Collider2D> collider;

		std::set<RigidBody2D*> newCollisions;

		std::set<RigidBody2D*> oldCollisions;

		friend class Physics::PhysicsEngine2D;
	};
}
