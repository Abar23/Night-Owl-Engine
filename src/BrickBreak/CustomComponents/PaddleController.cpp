#include "PaddleController.h"
#include "SideWall.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/GameObject/GameObject.h"

void BrickBreak::PaddleController::Awake()
{
	rigidBody = gameObject->GetComponent<NightOwl::Component::RigidBody2D>();
	hasCollidedWithWall = false;
}

void BrickBreak::PaddleController::Start()
{

}

void BrickBreak::PaddleController::Update()
{
	bool arrowKeyHeld = false;
	NightOwl::Math::Vec2F velocity = rigidBody->GetVelocity();
	NightOwl::Math::Vec3F position = gameObject->GetTransform()->GetPosition();

	if(NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyLeft) &&
	   position.x > -168.0f + PADDLE_HALF_WIDTH + WALL_WIDTH)
	{
		velocity.x = -120.0f;
		arrowKeyHeld = true;
	}

	if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyRight) &&
		position.x < 168.0f - PADDLE_HALF_WIDTH - WALL_WIDTH)
	{
		velocity.x = 120.0f;
		arrowKeyHeld = true;
	}

	if (!arrowKeyHeld)
	{
		rigidBody->SetVelocity({ 0, 0 });
	}
	else
	{
		rigidBody->SetVelocity(velocity);
	}

	if(NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeySpace) &&
	   gameObject->GetTransform()->HasChildren())
	{
		NightOwl::GameObject::GameObject& ball = gameObject->GetTransform()->GetChildAtIndex(0)->GetGameObject();

		ball.GetTransform()->RemoveParent();
		ball.GetComponent<NightOwl::Component::RigidBody2D>()->SetVelocity(NightOwl::Math::Vec2F(20.0, 200.5));
	}
}

void BrickBreak::PaddleController::OnEnable()
{

}

void BrickBreak::PaddleController::OnDisable()
{

}

void BrickBreak::PaddleController::OnCollisionStay2D(const NightOwl::Physics::Collision2D& collision2D)
{

}
