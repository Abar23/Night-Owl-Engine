#include "IkTargetController.h"

#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void IkTargetController::Start()
{
	speed = 1.0f;

	transform = gameObject->GetTransform();
	// transform->Translate({ 0.0f, 1.5f, 0.0f }, NightOwl::Space::World);
	transform->Scale(32.0f, NightOwl::Space::World);
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
