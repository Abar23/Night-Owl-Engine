#include "CollisionTest.h"
#include "NightOwl/Physics/Integrator/RungeKuttaIntegrator.h"
#include "PhysicsEngine2D.h"

namespace NightOwl::Physics
{
	void PhysicsEngine2D::Update()
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

	void PhysicsEngine2D::RemoveRigidBody2D(Component::RigidBody2D* rigidBody2D)
	{
		int rigidBody2DIndex = 0;
		for (; rigidBody2DIndex < rigidBodies.size(); ++rigidBody2DIndex)
		{
			if(rigidBodies[rigidBody2DIndex] == rigidBody2D)
			{
				break;
			}
		}

		if(rigidBody2DIndex != rigidBodies.size())
		{
			rigidBodies.erase(rigidBodies.begin() + rigidBody2DIndex);
		}
	}
}
