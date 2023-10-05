#include "NightOwlPch.h"

#include "RungeKuttaIntegrator.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	void RungeKuttaIntegrator::Integrate2D(RigidBody2D* rigidBody2D)
	{
		Vec2F linearAcceleration = rigidBody2D->GetAccumulatedForces() * rigidBody2D->GetInverseMass();

		Vec2F newPosition;
		Vec2F newLinearVelocity;

		IntegrateLinearVelocity2D(rigidBody2D, linearAcceleration, newPosition, newLinearVelocity);

		rigidBody2D->SetVelocity(newLinearVelocity);
		rigidBody2D->GetGameObject().GetTransform()->Translate(newPosition.x, newPosition.y, 0, Space::Local);
		rigidBody2D->ClearForces();
	}

	void RungeKuttaIntegrator::IntegrateLinearVelocity2D(const RigidBody2D* rigidBody2D, const Vec2F& linearAcceleration, Vec2F& newPosition, Vec2F& newlinearVelocity)
	{
		Vec2F k1, k2, k3, k4;
		float deltaTime = Time::GetDeltaTime();

		k1 = linearAcceleration * deltaTime;
		k2 = (linearAcceleration + k1) * 0.5f * deltaTime;
		k3 = (linearAcceleration + k2) * 0.5f * deltaTime;
		k4 = (linearAcceleration + k3) * deltaTime;

		newlinearVelocity = rigidBody2D->GetVelocity() + (k1 + k2 * 2 + k3 * 2 + k4) * (deltaTime / 6.0f);

		newPosition = rigidBody2D->GetPosition() + newlinearVelocity * deltaTime;
	}
}
