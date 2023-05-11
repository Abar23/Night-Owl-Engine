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

		std::shared_ptr<Component> Clone() override;

		~RigidBody2D() override;

		void AddForce(Math::Vec2F forceVector);

		void AddImpulse(const Math::Vec2F& impulseVector);

		Math::Vec2F GetVelocity() const;

		void SetVelocity(const Math::Vec2F& velocity);

		float GetRotation() const;

		void SetRotation(float rotation);

		float GetMass() const;

		void SetMass(float mass);

		float GetInverseMass() const;

		Math::Vec2F GetPosition() const;

		void SetPosition(const Math::Vec2F& position);

		Math::Vec2F GetDifferedPositionChange() const;

		void ResetDifferedPositionChange();

		void AddDifferedPositionChange(Math::Vec2F positionChange);

		Math::Vec2F GetAccumulatedForces() const;

		Math::Vec2F GetAccumulatedImpulses() const;

		void ClearForces();

		void ClearImpulses();

		Core::WeakPointer <Physics::Collider2D> GetCollider();

		void SetCollider(NightOwl::Core::WeakPointer <Physics::Collider2D> collider);

		bool IsKinematic() const;

		void SetIsKinematic(bool isKinematic);

		bool IsGravitational() const;

		void SetIsGravitational(bool isGravitational);

		bool IsTrigger() const;

		void SetIsTrigger(bool isTrigger);

		float GetGravityScale() const;

		void SetGravityScale(float gravityScale);

		float GetDragCoefficient() const;

		void SetDragCoefficient(float dragCoefficient);

		float GetCoefficientOfRestitution() const;

		void SetCoefficientOfRestitution(float coefficientOfRestitution);

		Math::Vec2F GetColliderPosition();

		REFLECT()

	private:
		Math::Vec2F velocity;

		bool isGravitational;

		bool isKinematic;

		bool isTrigger;

		float rotation; // z-axis only

		float mass;

		float inverseMass;

		float gravityScale;

		float dragCoefficient;

		float coefficientOfRestitution;

		Math::Vec2F position;

		Math::Vec2F differedPositionChange;

		Math::Vec2F linearVelocity;

		Math::Vec2F accumulatedForces;

		Math::Vec2F accumulatedImpulses;

		int impulseCounter;

		std::shared_ptr<Physics::Collider2D> collider;

		std::set<Core::WeakPointer<RigidBody2D>> newCollisions;

		std::set<Core::WeakPointer<RigidBody2D>> oldCollisions;

		friend class Physics::PhysicsEngine2D;

		friend class Physics::Collider2D;

		void Remove() override;
	};
}
