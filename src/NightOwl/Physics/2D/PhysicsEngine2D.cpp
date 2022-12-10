#include "PhysicsEngine2D.h"
#include "CollisionTest.h"
#include "NightOwl/Physics/Integrator/RungeKuttaIntegrator.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Physics
{
	void PhysicsEngine2D::Update()
	{
		// Collision, integration
		for (auto* rigidBody2D : rigidBodies)
		{
			if(rigidBody2D->GetGameObject().IsActive())
			{
				Collider2D* collider = rigidBody2D->GetCollider();
				for (auto* otherRigidBody2D : rigidBodies)
				{
					if (rigidBody2D != otherRigidBody2D)
					{
						Collider2D* otherCollider = otherRigidBody2D->GetCollider();
						if (CollisionTest::TestCollision(collider, otherCollider))
						{
							collidingBodies.insert(otherRigidBody2D);
						}
					}
				}

				// propagate collision events
				if (!collidingBodies.empty())
				{

					for(auto* otherRigidBody : collidingBodies)
					{
						if (!rigidBody2D->newCollisions.contains(otherRigidBody))
						{
							for (auto* owlBehavior : rigidBody2D->gameObject->owlBehaviorList)
							{
								Collision2D collision(rigidBody2D->GetCollider(), otherRigidBody->GetCollider());
								owlBehavior->OnCollisionEnter2D(collision);
							}
							rigidBody2D->newCollisions.insert(otherRigidBody);
						}
					}

					if (!rigidBody2D->newCollisions.empty())
					{
						for (auto collidingBody = rigidBody2D->newCollisions.begin(); collidingBody != rigidBody2D->newCollisions.end();)
						{
							if (collidingBodies.contains(*collidingBody))
							{
								for (auto* owlBehavior : rigidBody2D->gameObject->owlBehaviorList)
								{
									Collision2D collision(rigidBody2D->GetCollider(), (*collidingBody)->GetCollider());
									owlBehavior->OnCollisionStay2D(collision);
								}
								++collidingBody;
							}
							else
							{
								for (auto* owlBehavior : rigidBody2D->gameObject->owlBehaviorList)
								{
									Collision2D collision(rigidBody2D->GetCollider(), (*collidingBody)->GetCollider());
									owlBehavior->OnCollisionExit2D(collision);
								}
								collidingBody = rigidBody2D->newCollisions.erase(collidingBody);
							}
						}
					}
					collidingBodies.clear();
				}
				else
				{
					for (auto collidingBody = rigidBody2D->newCollisions.begin(); collidingBody != rigidBody2D->newCollisions.end();)
					{
						for (auto* owlBehavior : rigidBody2D->gameObject->owlBehaviorList)
						{
							Collision2D collision(rigidBody2D->GetCollider(), (*collidingBody)->GetCollider());
							owlBehavior->OnCollisionExit2D(collision);
						}
						collidingBody = rigidBody2D->newCollisions.erase(collidingBody);
					}
				}

				RungeKuttaIntegrator::Integrate2D(rigidBody2D);
			}
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
