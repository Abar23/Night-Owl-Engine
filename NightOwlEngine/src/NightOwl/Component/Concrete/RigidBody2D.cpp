#include <NightOwlPch.h>

#include "RigidBody2D.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	RigidBody2D::RigidBody2D()
		:	Component(ComponentType::RigidBody2D),
			isGravitational(false),
			isKinematic(false),
			isTrigger(false),
			rotation(0.0f),
			mass(0.0f),
			inverseMass(0.0f),
			gravityScale(1.0f),
			dragCoefficient(0.0f),
			coefficientOfRestitution(0.0f),
			position(0.0f),
			linearVelocity(0.0f),
			accumulatedForces(0.0f),
			impulseCounter(0),
			collider(nullptr)
	{
		PhysicsEngine2DLocator::GetPhysicsEngine2D()->AddRigidBody2D(this);
	}

	std::shared_ptr<Component> RigidBody2D::Clone()
	{
		std::shared_ptr<RigidBody2D> clone = std::make_shared<RigidBody2D>();

		clone->velocity = velocity;
		clone->rotation = rotation;
		clone->mass = mass;
		clone->inverseMass = inverseMass;
		clone->position = position;
		clone->linearVelocity = linearVelocity;
		clone->accumulatedForces = accumulatedForces;
		clone->isGravitational = isGravitational;
		clone->isKinematic = isKinematic;
		clone->isTrigger = isTrigger;
		clone->dragCoefficient = dragCoefficient;
		clone->gravityScale = gravityScale;
		clone->coefficientOfRestitution = coefficientOfRestitution;

		if(collider != nullptr)
		{
			//clone->collider = collider->Clone();
			clone->collider->rigidBody2D = clone.get();
		}
		
		return clone;
	}

	RigidBody2D::~RigidBody2D()
	{
		PhysicsEngine2DLocator::GetPhysicsEngine2D()->RemoveRigidBody2D(this);
	}

	void RigidBody2D::AddForce(Vec2F forceVector)
	{
		accumulatedForces += forceVector;
	}

	void RigidBody2D::AddImpulse(const Vec2F& impulseVector)
	{
		++impulseCounter;
		accumulatedImpulses += impulseVector;
	}

	Vec2F RigidBody2D::GetVelocity() const
	{
		return velocity;
	}

	void RigidBody2D::SetVelocity(const Vec2F& velocity)
	{
		this->velocity = velocity;
	}

	float RigidBody2D::GetRotation() const
	{
		return rotation;
	}

	void RigidBody2D::SetRotation(float rotation)
	{
		this->rotation = rotation;
	}

	float RigidBody2D::GetMass() const
	{
		return mass;
	}

	void RigidBody2D::SetMass(float mass)
	{
		this->mass = mass;
		this->inverseMass = 1.0f / mass;
	}

	float RigidBody2D::GetInverseMass() const
	{
		return inverseMass;
	}

	Vec2F RigidBody2D::GetPosition() const
	{
		return gameObject->GetTransform()->GetPosition().xy;
	}

	void RigidBody2D::SetPosition(const Vec2F& position)
	{
		this->position = position;
	}

	Vec2F RigidBody2D::GetDifferedPositionChange() const
	{
		return differedPositionChange;

	}

	void RigidBody2D::ResetDifferedPositionChange()
	{
		differedPositionChange = Vec2F::Zero();
	}

	void RigidBody2D::AddDifferedPositionChange(Vec2F positionChange)
	{
		if (signbit(differedPositionChange.x) ^ signbit(positionChange.x))
		{
			differedPositionChange.x += positionChange.x;
		} else
		{
			differedPositionChange.x = std::max(differedPositionChange.x, positionChange.x);

		}

		if (signbit(differedPositionChange.y) ^ signbit(positionChange.y))
		{
			differedPositionChange.y += positionChange.y;
		}
		else
		{
			differedPositionChange.y = std::max(differedPositionChange.y, positionChange.y);

		}
	}

	Vec2F RigidBody2D::GetAccumulatedForces() const
	{
		return accumulatedForces;
	}

	Vec2F RigidBody2D::GetAccumulatedImpulses() const
	{
		if (impulseCounter)
		{
			return accumulatedImpulses / static_cast<float>(impulseCounter);
		}
		return accumulatedImpulses;
	}

	void RigidBody2D::ClearForces()
	{
		accumulatedForces = Vec2F(0.0f);
	}

	void RigidBody2D::ClearImpulses()
	{
		accumulatedImpulses = Vec2F(0.0f);
		impulseCounter = 0;
	}

	Collider2D* RigidBody2D::GetCollider()
	{
		return collider.get();
	}

	bool RigidBody2D::IsKinematic() const
	{
		return isKinematic;
	}

	void RigidBody2D::SetIsKinematic(bool isKinematic)
	{
		this->isKinematic = isKinematic;
	}

	bool RigidBody2D::IsGravitational() const
	{
		return isGravitational;
	}

	void RigidBody2D::SetIsGravitational(bool isGravitational)
	{
		this->isGravitational = isGravitational;
	}

	bool RigidBody2D::IsTrigger() const
	{
		return isTrigger;
	}

	void RigidBody2D::SetIsTrigger(bool isTrigger)
	{
		this->isTrigger = isTrigger;
	}

	float RigidBody2D::GetGravityScale() const
	{
		return gravityScale;
	}

	void RigidBody2D::SetGravityScale(float gravityScale)
	{
		this->gravityScale = gravityScale;
	}

	void RigidBody2D::SetCollider(Collider2D* collider)
	{
		this->collider = std::shared_ptr<Collider2D>(collider);
		this->collider->rigidBody2D = this;
	}

	void RigidBody2D::Remove()
	{
		PhysicsEngine2DLocator::GetPhysicsEngine2D()->RemoveRigidBody2D(this);
	}

	float RigidBody2D::GetDragCoefficient() const
	{
		return dragCoefficient;
	}

	void RigidBody2D::SetDragCoefficient(float dragCoefficient)
	{
		this->dragCoefficient = dragCoefficient;
	}

	float RigidBody2D::GetCoefficientOfRestitution() const
	{
		return coefficientOfRestitution;
	}

	void RigidBody2D::SetCoefficientOfRestitution(float coefficientOfRestitution)
	{
		this->coefficientOfRestitution = coefficientOfRestitution;
	}

	Vec2F RigidBody2D::GetColliderPosition()
	{
		ENGINE_ASSERT(collider != nullptr, "Tried to get collider position on RigidBody2D without collider!");

		const Vec2F colliderOffset = collider->GetOffsetFromCenterOfObject();

		return GetPosition() + colliderOffset;
	}

	START_REFLECTION(RigidBody2D)
	CLASS_MEMBER_REFLECTION(velocity)
	CLASS_MEMBER_REFLECTION(mass)
	CLASS_MEMBER_REFLECTION(inverseMass)
	CLASS_MEMBER_REFLECTION(position)
	CLASS_MEMBER_REFLECTION(linearVelocity)
	CLASS_MEMBER_REFLECTION(accumulatedForces)
	//CLASS_MEMBER_REFLECTION(collider)
	END_REFLECTION(RigidBody2D)
}
