#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl::Component
{
	class RigidBody2D : public Component
	{
	public:
		RigidBody2D(GameObject::GameObject* gameObject);

		Math::Vec2F GetVelocity() const;

		void SetVelocity(const Math::Vec2F& velocity);

		float GetRotation() const;

		void SetRotation(float rotation);

		float GetMass() const;

		void SetMass(float mass);

		float GetInverseMass() const;

		void SetInverseMass(float inverseMass);

	private:
		Math::Vec2F velocity;

		float rotation; // z-axis only

		float mass;

		float inverseMass;
	};
}
