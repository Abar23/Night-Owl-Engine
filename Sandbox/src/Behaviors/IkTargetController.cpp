#include "IkTargetController.h"

#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void IkTargetController::Start()
{
	speed = 5.0f;

	transform = gameObject->GetTransform();
	transform->SetPosition({ 0.0f, 2.5f, 1.0f });
	transform->Scale(NightOwl::Vec3F(0.075f), NightOwl::Space::Local);
	originalLocation = transform->GetPosition();
}

void IkTargetController::Update()
{
	NightOwl::Vec3F directionToMoveTarget;

	int leftRightHeld = 0;
	int upDownHeld = 0;
	int forwardBackHeld = 0;

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::W))
	{
		forwardBackHeld--;
		directionToMoveTarget -= transform->GetForward() * speed;
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::S))
	{
		forwardBackHeld++;
		directionToMoveTarget += transform->GetForward() * speed;
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::A))
	{
		leftRightHeld--;
		directionToMoveTarget -= transform->GetRight() * speed;
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::D))
	{
		leftRightHeld++;
		directionToMoveTarget += transform->GetRight() * speed;
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::Q))
	{
		upDownHeld++;
		directionToMoveTarget += transform->GetUp() * speed;
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::E))
	{
		upDownHeld--;
		directionToMoveTarget -= transform->GetUp() * speed;
	}

	int directionsHeld = std::abs(upDownHeld) + std::abs(forwardBackHeld) + std::abs(leftRightHeld);
	if (directionsHeld > 1)
	{
		directionToMoveTarget /= static_cast<float>(directionsHeld);
	}

	transform->Translate(directionToMoveTarget * NightOwl::Time::GetDeltaTime(), NightOwl::Space::Local);
}

void IkTargetController::SetSpeed(float speed)
{
	this->speed = speed;
}

void IkTargetController::Reset()
{
	transform->SetPosition(originalLocation);
}
