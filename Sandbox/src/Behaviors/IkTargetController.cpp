#include "IkTargetController.h"

#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void IkTargetController::Start()
{
	speed = 5.0f;

	transform = gameObject->GetTransform();
	transform->SetPosition({ 0.0f, 2.5f, 1.0f });
	transform->Scale(NightOwl::Vec3F(0.15f), NightOwl::Space::Local);
}

void IkTargetController::Update()
{
	const float deltaTime = NightOwl::Time::GetDeltaTime();

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::W))
	{
		transform->Translate(transform->GetForward() * speed * deltaTime, NightOwl::Space::Local);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::S))
	{
		transform->Translate(transform->GetForward() * -1.0f * speed * deltaTime, NightOwl::Space::Local);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::A))
	{
		transform->Translate(transform->GetRight() * -1.0f * speed * deltaTime, NightOwl::Space::Local);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::D))
	{
		transform->Translate(transform->GetRight() * speed * deltaTime, NightOwl::Space::Local);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::Q))
	{
		transform->Translate(transform->GetUp() * speed * deltaTime, NightOwl::Space::Local);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::E))
	{
		transform->Translate(transform->GetUp() * -1.0f * speed * deltaTime, NightOwl::Space::Local);
	}
}

void IkTargetController::SetSpeed(float speed)
{
	this->speed = speed;
}
