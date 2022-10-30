#include <iostream>
#include "OwlBehaviorTest.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void OwlBehaviorTest::Awake()
{
	std::cout << "Awake" << std::endl;
	rigidBody2D = gameObject->GetComponent<NightOwl::Component::RigidBody2D>();
}

void OwlBehaviorTest::Start()
{
	std::cout << "Start" << std::endl;
}

void OwlBehaviorTest::Update()
{
	std::cout << "Update" << std::endl;
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
	std::cout << "Enable" << std::endl;
}

void OwlBehaviorTest::OnDisable()
{
	std::cout << "Disable" << std::endl;
}
