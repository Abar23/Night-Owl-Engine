#include "BottomWall.h"

#include "Ball.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"

namespace BrickBreak
{
	void BottomWall::Awake()
	{
		hasCollidedWithBall = false;
	}

	void BottomWall::Start()
	{

	}

	void BottomWall::Update()
	{

	}

	void BottomWall::OnEnable()
	{

	}

	void BottomWall::OnDisable()
	{

	}

	void BottomWall::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{
		if(collision2D.GetOtherBody()->GetGameObject().HasComponent<Ball>())
		{
			ENGINE_LOG_DEBUG("Got in the bottom wall collision");
			hasCollidedWithBall = true;
		}
	}

	bool BottomWall::HasCollidedWithBall()
	{
		return hasCollidedWithBall;
	}

	void BottomWall::SetHasCollidedWithBall(bool value)
	{
		hasCollidedWithBall = value;
	}
}
