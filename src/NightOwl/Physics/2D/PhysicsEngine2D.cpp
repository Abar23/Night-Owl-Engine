#include "CollisionTest.h"
#include "NightOwl/Physics/Integrator/RungeKuttaIntegrator.h"
#include "PhysicsEngine2D.h"

namespace NightOwl::Physics
{
	void PhysicsEngine2D::Update() const
	{
		for (auto* rigidBody2D : rigidBodies)
		{
			Collider2D* collider = rigidBody2D->GetCollider();
			for (auto* otherRigidBody2D : rigidBodies)
			{
				if(rigidBody2D != otherRigidBody2D)
				{
					Collider2D* otherCollider = otherRigidBody2D->GetCollider();
					CollisionTest::TestCollision(collider, otherCollider);
				}
			}

			RungeKuttaIntegrator::Integrate2D(rigidBody2D);
		}
	}

	void PhysicsEngine2D::AddRigidBody2D(Component::RigidBody2D* rigidBody2D)
	{
		rigidBodies.push_back(rigidBody2D);
	}

	void PhysicsEngine2D::RemoveRigidBody2D(const Component::RigidBody2D* rigidBody2D)
	{
		int rigidBody2DIndex = 0;
		for (const auto* rigidBody : rigidBodies)
		{
			if(rigidBody == rigidBody2D)
			{
				break;
			}
			rigidBody2DIndex++;
		}

		if(rigidBody2DIndex != rigidBodies.size())
		{
			rigidBodies.erase(rigidBodies.begin() + rigidBody2DIndex);
		}
	}
}
