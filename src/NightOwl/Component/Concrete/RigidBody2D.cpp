#include "RigidBody2D.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"

namespace NightOwl::Component
{
	RigidBody2D::RigidBody2D()
		:	Component(ComponentType::RigidBody2D),
			rotation(0.0f),
			mass(0.0f),
			inverseMass(0.0f),
			position(0.0f),
			linearVelocity(0.0f),
			accumulatedForces(0.0f),
			collider(nullptr)
	{
		Core::PhysicsEngine2DLocator::GetPhysicsEngine2D()->AddRigidBody2D(this);
	}

	RigidBody2D::~RigidBody2D()
	{
		Core::PhysicsEngine2DLocator::GetPhysicsEngine2D()->RemoveRigidBody2D(this);
	}

	void RigidBody2D::AddForce(Math::Vec2F forceVector)
	{
		accumulatedForces += forceVector;
	}

	Math::Vec2F RigidBody2D::GetVelocity() const
	{
		return velocity;
	}

	void RigidBody2D::SetVelocity(const Math::Vec2F& velocity)
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

	Math::Vec2F RigidBody2D::GetPosition() const
	{
		return position;
	}

	void RigidBody2D::SetPosition(const Math::Vec2F& position)
	{
		this->position = position;
	}

	Math::Vec2F RigidBody2D::GetAccumulatedForces() const
	{
		return accumulatedForces;
	}

	void RigidBody2D::ClearForces()
	{
		accumulatedForces = Math::Vec2F(0.0f);
	}

	Physics::Collider2D* RigidBody2D::GetCollider()
	{
		return collider.get();
	}

	void RigidBody2D::SetCollider(Physics::Collider2D* collider)
	{
		this->collider = std::shared_ptr<Physics::Collider2D>(collider);
		this->collider->rigidBody2D = this;
	}
}
