#pragma once

#include <GLFW/glfw3.h>

namespace NightOwl
{
	enum class GamePadButton
	{
		A,
		B,
		X,
		Y,
		LeftBumper,
		RightBumper,
		Back,
		Start,
		Guide,
		LeftThumb,
		RightThumb,
		DPadUp,
		DPadRight,
		DPadDown,
		DPadLeft,
		NumberOfSupportedGamePadButtons,
		None,
		Last = DPadLeft,
		Cross = A,
		Circle = B,
		Square = X,
		Triangle = Y
	};

	inline int NightOwlGamePadButtonToGlfwGamePadButton(GamePadButton gamePadButton)
	{
		int glfwGamePadButton;

		switch (gamePadButton)
		{
		case GamePadButton::A:
		case GamePadButton::B:
		case GamePadButton::X:
		case GamePadButton::Y:
		case GamePadButton::LeftBumper:
		case GamePadButton::RightBumper:
		case GamePadButton::Back:
		case GamePadButton::Start:
		case GamePadButton::Guide:
		case GamePadButton::LeftThumb:
		case GamePadButton::RightThumb:
		case GamePadButton::DPadUp:
		case GamePadButton::DPadRight:
		case GamePadButton::DPadDown:
		case GamePadButton::DPadLeft:
			glfwGamePadButton = static_cast<int>(gamePadButton) + GLFW_GAMEPAD_BUTTON_A - static_cast<int>(GamePadButton::A);
			break;

		default:
			glfwGamePadButton = -1;
			break;
		}

		return glfwGamePadButton;
	}

	inline GamePadButton GlfwGamePadButtonToNightOwlGamePadButton(int glfwGamePadButton)
	{
		GamePadButton nightOwlGamePadButton;

		switch (glfwGamePadButton)
		{
		case GLFW_GAMEPAD_BUTTON_A:
		case GLFW_GAMEPAD_BUTTON_B:
		case GLFW_GAMEPAD_BUTTON_X:
		case GLFW_GAMEPAD_BUTTON_Y:
		case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
		case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
		case GLFW_GAMEPAD_BUTTON_BACK:
		case GLFW_GAMEPAD_BUTTON_START:
		case GLFW_GAMEPAD_BUTTON_GUIDE:
		case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
		case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
		case GLFW_GAMEPAD_BUTTON_DPAD_UP:
		case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
		case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
		case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
			nightOwlGamePadButton = static_cast<GamePadButton>(static_cast<int>(GamePadButton::A) + glfwGamePadButton - GLFW_GAMEPAD_BUTTON_A);
			break;

		default:
			nightOwlGamePadButton = GamePadButton::None;
			break;
		}

		return nightOwlGamePadButton;
	}
}
