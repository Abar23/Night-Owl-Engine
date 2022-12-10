#include "Ball.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "Brick.h"
#include "PaddleController.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Math/Math.h"

namespace BrickBreak
{
	void Ball::Awake()
	{
		rigidBody = gameObject->GetComponent<NightOwl::Component::RigidBody2D>();
	}

	void Ball::Start()
	{

	}

	void Ball::Update()
	{
	}

	void Ball::OnEnable()
	{

	}

	void Ball::OnDisable()
	{

	}

	void Ball::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{
		if(collision2D.GetOtherBody()->GetGameObject().IsActive())
		{
			NightOwl::Math::Vec2F velocity = rigidBody->GetVelocity();
			NightOwl::Math::Vec3F collisionDirection = gameObject->GetTransform()->GetPosition() - collision2D.GetOtherBody()->GetGameObject().GetTransform()->GetPosition();

			if(collision2D.GetOtherBody()->GetGameObject().HasComponent<Brick>())
			{
				if (std::abs(collisionDirection.x) > BRICK_HALF_WIDTH)
				{
					velocity.x *= -1.0f;
				}

				if (std::abs(collisionDirection.y) > BRICK_HALF_HEIGHT)
				{
					if(collisionDirection.y < 0.0f)
					{
						velocity.y = -std::abs(velocity.y);
					}
					else
					{
						velocity.y = std::abs(velocity.y);
					}
				}

				collision2D.GetOtherBody()->GetGameObject().SetActive(false);

				gameObject->GetScene()->FindGameObject("Brick Manager")->GetComponent<BrickManager>()->DecreaseNumberOfActiveBricks();
			}

			if (collision2D.GetOtherBody()->GetGameObject().HasComponent<PaddleController>())
			{
				NightOwl::Math::Vec2F up = NightOwl::Math::Vec2F::Up();
				float angle = NightOwl::Math::RadToDegrees(std::atan2(collisionDirection.x * up.y - collisionDirection.y * up.x, NightOwl::Math::Vec2F::Dot(collisionDirection.xy, up)));
				angle /= 63.43f;
				velocity.x = angle * 100.0f;
				velocity.y *= -1;
				ENGINE_LOG_DEBUG("Angle between vectors is {0}", velocity.x);
			}

			rigidBody->SetVelocity(velocity);
		}
	}
}
