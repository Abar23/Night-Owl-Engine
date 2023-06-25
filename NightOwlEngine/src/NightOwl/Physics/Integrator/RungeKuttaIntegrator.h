#pragma once

#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl
{
	class RungeKuttaIntegrator
	{
	public:
		static void Integrate2D(RigidBody2D* rigidBody2D);

		static void IntegrateLinearVelocity2D(const RigidBody2D* rigidBody2D, const Vec2F& linearAcceleration, Vec2F& newPosition, Vec2F& newlinearVelocity);
	};
}
