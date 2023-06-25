#pragma once

#include "GLFW/glfw3.h"

namespace NightOwl
{
	enum class InputAction
	{
		None,
		Pressed,
		Released,
		Repeated,
		Held
	};

	inline int NightOwlKeyActionToGlfwKeyAction(InputAction nightOwlInputAction)
	{
		int glfwKeyAction;

		switch (nightOwlInputAction)
		{
		case InputAction::Pressed:
			glfwKeyAction = GLFW_PRESS;
			break;

		case InputAction::Released:
			glfwKeyAction = GLFW_RELEASE;
			break;

		case InputAction::Repeated:
			glfwKeyAction = GLFW_REPEAT;
			break;

		default:
			glfwKeyAction = -1;
			break;
		}

		return glfwKeyAction;
	}

	inline InputAction GlfwInputActionToNightOwlKeyAction(int glfwKeyAction)
	{
		InputAction nightOwlInputAction;

		switch (glfwKeyAction)
		{
		case GLFW_PRESS:
			nightOwlInputAction = InputAction::Pressed;
			break;

		case GLFW_RELEASE:
			nightOwlInputAction = InputAction::Released;
			break;

		case GLFW_REPEAT:
			nightOwlInputAction = InputAction::Repeated;
			break;

		default:
			nightOwlInputAction = InputAction::None;
			break;
		}

		return nightOwlInputAction;
	}
}
