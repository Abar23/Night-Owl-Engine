#pragma once

#include "GLFW/glfw3.h"

namespace NightOwl::Input
{
	enum class GamePadAxis
	{
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY,
		LeftTrigger,
		RightTrigger,
		Last = RightTrigger,
		None
	};

	inline int NightOwlGamePadAxisToGlfwGamePadAxis(GamePadAxis gamePadAxis)
	{
		int glfwGamePadAxis;

		switch (gamePadAxis)
		{
		case GamePadAxis::LeftStickX:
		case GamePadAxis::LeftStickY:
		case GamePadAxis::RightStickX:
		case GamePadAxis::RightStickY:
		case GamePadAxis::LeftTrigger:
		case GamePadAxis::RightTrigger:
			glfwGamePadAxis = static_cast<int>(gamePadAxis) + GLFW_GAMEPAD_AXIS_LEFT_X - static_cast<int>(GamePadAxis::LeftStickX);
			break;

		case GamePadAxis::None:
			glfwGamePadAxis = -1;
		}

		return glfwGamePadAxis;
	}

	inline GamePadAxis GlfwGamePadAxisToNightOwlGamePadAxis(int glfwGamePadAxis)
	{
		GamePadAxis nightOwlGamePadAxis;

		switch (glfwGamePadAxis)
		{
		case GLFW_GAMEPAD_AXIS_LEFT_X:
		case GLFW_GAMEPAD_AXIS_LEFT_Y:
		case GLFW_GAMEPAD_AXIS_RIGHT_X:
		case GLFW_GAMEPAD_AXIS_RIGHT_Y:
		case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
		case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
			nightOwlGamePadAxis = static_cast<GamePadAxis>(static_cast<int>(GamePadAxis::LeftStickX) + glfwGamePadAxis - GLFW_GAMEPAD_AXIS_LEFT_X);
			break;

		default:
			nightOwlGamePadAxis = GamePadAxis::None;
		}

		return nightOwlGamePadAxis;
	}
}
