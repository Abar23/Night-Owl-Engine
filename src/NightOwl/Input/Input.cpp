#include "Input.h"
#include "NightOwl/Input/InputAction.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl::Input
{
	using namespace std::placeholders;

	Input* Input::inputInstance{ nullptr };
	std::mutex Input::mutexLock;

	Input::Input()
		:	keyActionArray{ InputAction::None },
			mouseButtonActionArray{ InputAction::None },
			anyKeyHeldCounter(0),
			anyKeyPressedCounter(0),
			isKeyInputDirty(false),
			isMouseButtonInputDirty(false)
	{
		Window::WindowApi::GetWindow()->SetKeyCallback(std::bind(&Input::KeyCallback, _1, _2, _3, _4));
		Window::WindowApi::GetWindow()->SetMouseButtonCallback(std::bind(&Input::MouseButtonCallback, _1, _2, _3));
		Window::WindowApi::GetWindow()->SetMousePositionCallback(std::bind(&Input::MousePositionCallback, _1, _2));
		Window::WindowApi::GetWindow()->SetMouseScrollCallback(std::bind(&Input::MouseScrollCallback, _1, _2));
	}

	Input* Input::GetInstance()
	{
		std::lock_guard lock(mutexLock);
		if(inputInstance == nullptr)
		{
			inputInstance = new Input();
		}
		return inputInstance;
	}

	void Input::Init()
	{
		GetInstance();
	}

	void Input::Update()
	{
		if(inputInstance->isKeyInputDirty)
		{
			for (auto& keyIndex : inputInstance->keyActionArray)
			{
				if (keyIndex == InputAction::Pressed)
				{
					keyIndex = InputAction::Held;
					inputInstance->anyKeyHeldCounter++;
					inputInstance->anyKeyPressedCounter--;
				}
				else if (keyIndex == InputAction::Released)
				{
					keyIndex = InputAction::None;
					inputInstance->anyKeyHeldCounter--;
				}
			}
			inputInstance->isKeyInputDirty = false;
		}

		if (inputInstance->isMouseButtonInputDirty)
		{
			for (auto& mouseButtonIndex : inputInstance->mouseButtonActionArray)
			{
				if (mouseButtonIndex == InputAction::Pressed)
				{
					mouseButtonIndex = InputAction::Held;
					inputInstance->anyKeyHeldCounter++;
					inputInstance->anyKeyPressedCounter--;
				}
				else if (mouseButtonIndex == InputAction::Released)
				{
					mouseButtonIndex = InputAction::None;
					inputInstance->anyKeyHeldCounter--;
				}
			}
			inputInstance->isMouseButtonInputDirty = false;
		}
	}

	const Math::Vec2D& Input::GetScrollDelta() 
	{
		return inputInstance->scrollDelta;
	}

	bool Input::IsKeyPressed(KeyCode nightOwlKeyCode) 
	{
		return inputInstance->keyActionArray[static_cast<int>(nightOwlKeyCode)] == InputAction::Pressed;
	}

	bool Input::IsKeyRelease(KeyCode nightOwlKeyCode) 
	{
		return inputInstance->keyActionArray[static_cast<int>(nightOwlKeyCode)] == InputAction::Released;
	}

	bool Input::IsKeyHeld(KeyCode nightOwlKeyCode) 
	{
		return inputInstance->keyActionArray[static_cast<int>(nightOwlKeyCode)] == InputAction::Held;
	}

	bool Input::IsMouseButtonPressed(KeyCode nightOwlMouseButton) 
	{
		return inputInstance->mouseButtonActionArray[static_cast<int>(nightOwlMouseButton)] == InputAction::Pressed;
	}

	bool Input::IsMouseButtonRelease(KeyCode nightOwlMouseButton) 
	{
		return inputInstance->mouseButtonActionArray[static_cast<int>(nightOwlMouseButton)] == InputAction::Released;
	}

	bool Input::IsMouseButtonHeld(KeyCode nightOwlMouseButton) 
	{
		return inputInstance->mouseButtonActionArray[static_cast<int>(nightOwlMouseButton)] == InputAction::Held;
	}

	bool Input::IsAnyKeyHeld() 
	{
		return inputInstance->anyKeyHeldCounter > 0;
	}

	bool Input::IsAnyKeyPressed() 
	{
		return inputInstance->anyKeyPressedCounter > 0;
	}

	void Input::KeyCallback(int key, int scancode, int action, int mods)
	{
		ENGINE_LOG_INFO("Key {0} Scancode {1} Action {2} Mods {3}", key, scancode, action, mods);

		const KeyCode nightOwlKeyCode = GlfwKeyCodeToNightOwlKeyCode(key);
		const InputAction nightOwlInputAction = GlfwInputActionToNightOwlKeyAction(action);

		if(nightOwlInputAction != InputAction::Repeated)
		{
			inputInstance->keyActionArray[static_cast<int>(nightOwlKeyCode)] = nightOwlInputAction;
			inputInstance->isKeyInputDirty = true;

			if (nightOwlInputAction == InputAction::Pressed)
			{
				inputInstance->anyKeyPressedCounter++;
			}
		}
	}

	void Input::MouseButtonCallback(int button, int action, int mods)
	{
		ENGINE_LOG_INFO("Button {0} Action {1} Mods {2}", button, action, mods);

		const MouseButton nightOwlMouseButton = GlfwMouseButtonToNightOwlMouseButton(button);
		const InputAction nightOwlInputAction = GlfwInputActionToNightOwlKeyAction(action);

		if (nightOwlInputAction != InputAction::Repeated)
		{
			inputInstance->mouseButtonActionArray[static_cast<int>(nightOwlMouseButton)] = nightOwlInputAction;
			inputInstance->isMouseButtonInputDirty = true;

			if (nightOwlInputAction == InputAction::Pressed)
			{
				inputInstance->anyKeyPressedCounter++;
			}
		}
	}

	void Input::MousePositionCallback(double xPosition, double yPosition)
	{
		ENGINE_LOG_INFO("xPosition {0} yPosition {1}", xPosition, yPosition);

		inputInstance->mousePosition.x = xPosition;
		inputInstance->mousePosition.y = yPosition;
	}

	void Input::MouseScrollCallback(double xOffset, double yOffset)
	{
		ENGINE_LOG_INFO("xOffset {0} yOffset {1}", xOffset, yOffset);

		inputInstance->scrollDelta.x = xOffset;
		inputInstance->scrollDelta.y = yOffset;
	}
}
