#include "SideWall.h"

#include "Ball.h"
#include "PaddleController.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace BrickBreak
{
	void SideWall::Awake()
	{
		rigidBody = gameObject->GetComponent<NightOwl::Component::RigidBody2D>();
	}

	void SideWall::Start()
	{

	}

	void SideWall::Update()
	{

	}

	void SideWall::OnEnable()
	{

	}

	void SideWall::OnDisable()
	{

	}

	void SideWall::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{
		if(collision2D.GetOtherBody()->GetGameObject().HasComponent<Ball>())
		{
			NightOwl::Component::RigidBody2D* ballRigidBody = collision2D.GetOtherBody();
			NightOwl::Math::Vec2F ballVelocity = ballRigidBody->GetVelocity();
			NightOwl::Math::Vec3F collisionDirection = ballRigidBody->GetGameObject().GetTransform()->GetPosition() - gameObject->GetTransform()->GetPosition();

			if(collisionDirection.x < 0.0f && ballVelocity.x > 0.0f)
			{
				ballVelocity.x = -std::abs(ballVelocity.x);
			}
			else if(collisionDirection.x > 0.0f && ballVelocity.x < 0.0f)
			{
				ballVelocity.x = std::abs(ballVelocity.x);
			}

			ballRigidBody->SetVelocity(ballVelocity);
		}
	}
}
