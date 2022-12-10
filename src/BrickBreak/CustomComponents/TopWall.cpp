#include "TopWall.h"
#include "Ball.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace BrickBreak
{
	void TopWall::Awake()
	{

	}

	void TopWall::Start()
	{

	}

	void TopWall::Update()
	{

	}

	void TopWall::OnEnable()
	{

	}

	void TopWall::OnDisable()
	{

	}

	void TopWall::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{
		if(collision2D.GetOtherBody()->GetGameObject().HasComponent<Ball>())
		{
			ENGINE_LOG_DEBUG("Got here");

			NightOwl::Math::Vec2F ballVelocity = collision2D.GetOtherBody()->GetVelocity();

			ballVelocity.y = -std::abs(ballVelocity.y);

			collision2D.GetOtherBody()->SetVelocity(ballVelocity);
		}
	}
}
