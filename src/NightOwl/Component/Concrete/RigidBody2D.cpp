#include "RigidBody2D.h"

namespace NightOwl::Component
{
	RigidBody2D::RigidBody2D(GameObject::GameObject* gameObject)
		:	Component(ComponentType::RigidBody2D),
			rotation(0.0f),
			mass(0.0f),
			inverseMass(0.0f)
	{
		this->gameObject = gameObject;
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
	}

	float RigidBody2D::GetInverseMass() const
	{
		return inverseMass;
	}

	void RigidBody2D::SetInverseMass(float inverseMass)
	{
		this->inverseMass = inverseMass;
	}
}
