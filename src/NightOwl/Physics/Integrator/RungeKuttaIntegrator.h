#pragma once

#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl::Physics
{
	class RungeKuttaIntegrator
	{
	public:
		static void Integrate2D(Component::RigidBody2D* rigidBody2D);

		static void IntegrateLinearVelocity2D(const Component::RigidBody2D* rigidBody2D, const Math::Vec2F& linearAcceleration, Math::Vec2F& newPosition, Math::Vec2F& newlinearVelocity);
	};
}
