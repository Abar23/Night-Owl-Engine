#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Physics/2D/Collider/Collider2D.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include <memory>
#include <set>

namespace NightOwl
{
	class PhysicsEngine2D;

	class Collision2D;

	class RigidBody2D : public Component
	{
	public:
		RigidBody2D();

		std::shared_ptr<Component> Clone() override;

		~RigidBody2D() override;

		void AddForce(Vec2F forceVector);

		void AddImpulse(const Vec2F& impulseVector);

		Vec2F GetVelocity() const;

		void SetVelocity(const Vec2F& velocity);

		float GetRotation() const;

		void SetRotation(float rotation);

		float GetMass() const;

		void SetMass(float mass);

		float GetInverseMass() const;

		Vec2F GetPosition() const;

		void SetPosition(const Vec2F& position);

		Vec2F GetDifferedPositionChange() const;

		void ResetDifferedPositionChange();

		void AddDifferedPositionChange(Vec2F positionChange);

		Vec2F GetAccumulatedForces() const;

		Vec2F GetAccumulatedImpulses() const;

		void ClearForces();

		void ClearImpulses();

		Collider2D* GetCollider();

		void SetCollider(Collider2D* collider);

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

		Vec2F GetColliderPosition();

		REFLECT()

	private:
		Vec2F velocity;

		bool isGravitational;

		bool isKinematic;

		bool isTrigger;

		float rotation; // z-axis only

		float mass;

		float inverseMass;

		float gravityScale;

		float dragCoefficient;

		float coefficientOfRestitution;

		Vec2F position;

		Vec2F differedPositionChange;

		Vec2F linearVelocity;

		Vec2F accumulatedForces;

		Vec2F accumulatedImpulses;

		int impulseCounter;

		std::shared_ptr<Collider2D> collider;

		std::set<RigidBody2D*> newCollisions;

		std::set<RigidBody2D*> oldCollisions;

		friend class PhysicsEngine2D;

		friend class Collider2D;

		void Remove() override;
	};
}
