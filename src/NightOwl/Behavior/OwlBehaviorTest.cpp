#include <iostream>
#include "OwlBehaviorTest.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void OwlBehaviorTest::Awake()
{
	rigidBody2D = gameObject->GetComponent<NightOwl::Component::RigidBody2D>();
}

void OwlBehaviorTest::Start()
{
}

void OwlBehaviorTest::Update()
{
	if(NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeySpace))
	{
		isEnabled = false;
	}

	if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyUp))
	{
		rigidBody2D->AddForce(NightOwl::Math::Vec2F::Up() * 1000.0f);
	}

	if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyDown))
	{
		rigidBody2D->AddForce(NightOwl::Math::Vec2F::Down() * 1000.0f);
	}

	if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyRight))
	{
		rigidBody2D->AddForce(NightOwl::Math::Vec2F::Right() * 1000.0f);
	}

	if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyLeft))
	{
		rigidBody2D->AddForce(NightOwl::Math::Vec2F::Left() * 1000.0f);
	}
}

void OwlBehaviorTest::OnEnable()
{
}

void OwlBehaviorTest::OnDisable()
{
}

void OwlBehaviorTest::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
{
	std::cout << "Enter" << std::endl;
}

void OwlBehaviorTest::OnCollisionStay2D(const NightOwl::Physics::Collision2D& collision2D)
{
	std::cout << "Stay" << std::endl;
}

void OwlBehaviorTest::OnCollisionExit2D(const NightOwl::Physics::Collision2D& collision2D)
{
	std::cout << "Exit" << std::endl;
}
