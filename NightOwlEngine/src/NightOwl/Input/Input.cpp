#include <NightOwlPch.h>

#include "Input.h"
#include "NightOwl/Input/InputAction.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl
{
	using namespace std::placeholders;

	std::unique_ptr<Input> Input::inputInstance{nullptr};

	std::mutex Input::mutexLock;

	Input::Input()
		:	keyActionArray{ InputAction::None },
			mouseButtonActionArray{ InputAction::None },
			anyKeyHeldCounter(0),
			anyKeyPressedCounter(0),
			isKeyInputDirty(false),
			isMouseButtonInputDirty(false)
	{ }

	Input* Input::GetInstance()
	{
		std::lock_guard lock(mutexLock);
		if(inputInstance == nullptr)
		{
			inputInstance = std::unique_ptr<Input>(new Input());

			CheckForConnectedControllers();

			SetupWindowCallbacks();
		}
		return inputInstance.get();
	}

	void Input::Init()
	{
		GetInstance();
	}

	void Input::Update()
	{
		UpdateKeys();

		UpdateMouseButtons();

		UpdateGamePads();
	}

	const Vec2D& Input::GetScrollDelta() 
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

	bool Input::IsGamePadButtonPressed(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton)
	{
		if(!IsGamePadConnected(nightOwlGamePadId))
		{
			return false;
		}

		GamePadState& gamePadState = inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)];
		return gamePadState.GetButton(nightOwlGamePadButton) == InputAction::Pressed;
	}

	bool Input::HasAnyGamePadPressedButton(GamePadButton nightOwlGamePadButton)
	{
		return DoesAnyGameButtonHaveState(nightOwlGamePadButton, InputAction::Pressed);
	}

	bool Input::IsGamePadButtonRelease(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton)
	{
		if(!IsGamePadConnected(nightOwlGamePadId))
		{
			return false;
		}	

		GamePadState& gamePadState = inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)];
		return gamePadState.GetButton(nightOwlGamePadButton) == InputAction::Released;
	}

	bool Input::HasAnyGamePadReleasedButton(GamePadButton nightOwlGamePadButton)
	{
		return DoesAnyGameButtonHaveState(nightOwlGamePadButton, InputAction::Released);
	}

	bool Input::IsGamePadButtonHeld(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton)
	{
		if(!IsGamePadConnected(nightOwlGamePadId))
		{
			return false;
		}

		GamePadState& gamePadState = inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)];
		return gamePadState.GetButton(nightOwlGamePadButton) == InputAction::Held;
	}

	bool Input::HasAnyGamePadHeldButton(GamePadButton nightOwlGamePadButton)
	{
		return DoesAnyGameButtonHaveState(nightOwlGamePadButton, InputAction::Held);
	}

	bool Input::IsGamePadConnected(GamePadId nightOwlGamePadId)
	{
		ENGINE_ASSERT(nightOwlGamePadId != GamePadId::None, "GamePadId None given to Input System when asking if game pad is connected");

		return inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)].IsGamePadConnected();
	}

	float Input::GetGamePadAxis(GamePadId nightOwlGamePadId, GamePadAxis nightOwlGamePadAxis)
	{
		if(!IsGamePadConnected(nightOwlGamePadId))
		{
			return 0.0f;
		}	

		const auto& gamePadAxes = inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)].GetAxes();
		return gamePadAxes[static_cast<int>(nightOwlGamePadAxis)];
	}

	std::array<float, 4> Input::GetAxisOfAllGamePads(GamePadAxis nightOwlGamePadAxis)
	{
		std::array<float, 4> gamePadAxisValues = { 0.0f };

		for (int gamePadId = 0; gamePadId < static_cast<int>(GamePadId::Pad4); ++gamePadId)
		{
			gamePadAxisValues[gamePadId] = GetGamePadAxis(static_cast<GamePadId>(gamePadId), nightOwlGamePadAxis);
		}

		return gamePadAxisValues;
	}

	bool Input::IsAnyKeyHeld() 
	{
		return inputInstance->anyKeyHeldCounter > 0;
	}

	bool Input::IsAnyKeyPressed() 
	{
		return inputInstance->anyKeyPressedCounter > 0;
	}

	void Input::UpdateKeys()
	{
		if (!inputInstance->isKeyInputDirty)
		{
			return;
		}

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

	void Input::UpdateMouseButtons()
	{
		if (!inputInstance->isMouseButtonInputDirty)
		{
			return;
		}

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

	void Input::UpdateGamePads()
	{
		for (int gamePadIndex = 0; gamePadIndex < inputInstance->gamePads.size(); ++gamePadIndex)
		{
			GamePadState& currentGamePadState = inputInstance->gamePads[gamePadIndex];

			if (currentGamePadState.IsGamePadConnected())
			{
				GamePadState newGamePadState = WindowApi::GetWindow()->GetGamePadState(static_cast<GamePadId>(gamePadIndex));

				currentGamePadState.SetAxes(newGamePadState.GetAxes());

				auto& currentButtonStates = currentGamePadState.GetButtons();
				auto& newButtonStates = newGamePadState.GetButtons();

				for (int buttonIndex = 0; buttonIndex < currentButtonStates.size(); buttonIndex++)
				{
					auto& currentButtonState = currentButtonStates[buttonIndex];
					const auto& newButtonState = newButtonStates[buttonIndex];

					if (currentButtonState == InputAction::None && newButtonState == InputAction::Pressed)
					{
						currentButtonState = InputAction::Pressed;
						inputInstance->anyKeyPressedCounter++;
					}
					else if (currentButtonState == InputAction::Pressed && newButtonState == InputAction::Pressed)
					{
						currentButtonState = InputAction::Held;
						inputInstance->anyKeyHeldCounter++;
						inputInstance->anyKeyPressedCounter--;
					}
					else if ((currentButtonState == InputAction::Pressed || currentButtonState == InputAction::Held) && newButtonState == InputAction::Released)
					{
						currentButtonState = InputAction::Released;
					}
					else if (currentButtonState == InputAction::Released && newButtonState == InputAction::Released)
					{
						currentButtonState = InputAction::None;
						inputInstance->anyKeyHeldCounter--;
					}
				}
			}
		}
	}

	void Input::KeyCallback(int key, int scancode, int action, int mods)
	{
		const KeyCode nightOwlKeyCode = GlfwKeyCodeToNightOwlKeyCode(key);
		const InputAction nightOwlInputAction = GlfwInputActionToNightOwlKeyAction(action);

		if(nightOwlInputAction == InputAction::Repeated)
		{
			return;
		}

		inputInstance->keyActionArray[static_cast<int>(nightOwlKeyCode)] = nightOwlInputAction;
		inputInstance->isKeyInputDirty = true;

		if (nightOwlInputAction == InputAction::Pressed)
		{
			inputInstance->anyKeyPressedCounter++;
		}
	}

	void Input::MouseButtonCallback(int button, int action, int mods)
	{
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
		inputInstance->mousePosition.x = xPosition;
		inputInstance->mousePosition.y = yPosition;
	}

	void Input::MouseScrollCallback(double xOffset, double yOffset)
	{
		inputInstance->scrollDelta.x = xOffset;
		inputInstance->scrollDelta.y = yOffset;
	}

	void Input::GamePadConnectionCallback(int gamePadId, int connectionEventType)
	{
		const GamePadId nightOwlGamePadId = GlfwGamePadIdToNightOwlGamePadId(gamePadId);
		const GamePadConnection nightOwlGamePadConnection = GlfwGamePadConnectionToNightOwlGamePadConnection(connectionEventType);

		if (nightOwlGamePadId > GamePadId::Pad4)
		{
			return;
		}

		GamePadState& gamePad = inputInstance->gamePads[static_cast<int>(nightOwlGamePadId)];
		gamePad.SetGamePadConnectionStatus(nightOwlGamePadConnection);

		if (gamePad.IsGamePadConnected())
		{
			return;
		}

		for (auto& currentButton : gamePad.GetButtons())
		{
			if (currentButton == InputAction::Pressed)
			{
				inputInstance->anyKeyPressedCounter--;
			}

			if (currentButton == InputAction::Held)
			{
				inputInstance->anyKeyHeldCounter--;
			}

			currentButton = InputAction::None;
		}
	}

	bool Input::DoesAnyGameButtonHaveState(GamePadButton button, InputAction action)
	{
		bool isButtonHeld = false;

		for (auto& gamePadState : inputInstance->gamePads)
		{
			if (!gamePadState.IsGamePadConnected())
			{
				continue;
			}

			if (gamePadState.GetButton(button) == action)
			{
				isButtonHeld = true;
				break;
			}
		}

		return isButtonHeld;
	}

	void Input::CheckForConnectedControllers()
	{
		for (int gamePadStateIndex = 0; gamePadStateIndex < inputInstance->gamePads.size(); ++gamePadStateIndex)
		{
			const GamePadConnection status = WindowApi::GetWindow()->GetGamePadConnectionStatus(static_cast<GamePadId>(gamePadStateIndex));
			inputInstance->gamePads[gamePadStateIndex].SetGamePadConnectionStatus(status);
		}
	}

	void Input::SetupWindowCallbacks()
	{
		WindowApi::GetWindow()->SetKeyCallback(std::bind(&Input::KeyCallback, _1, _2, _3, _4));
		WindowApi::GetWindow()->SetMouseButtonCallback(std::bind(&Input::MouseButtonCallback, _1, _2, _3));
		WindowApi::GetWindow()->SetMousePositionCallback(std::bind(&Input::MousePositionCallback, _1, _2));
		WindowApi::GetWindow()->SetMouseScrollCallback(std::bind(&Input::MouseScrollCallback, _1, _2));
		WindowApi::GetWindow()->SetGamePadConnectionCallback(std::bind(&Input::GamePadConnectionCallback, _1, _2));
	}
}
