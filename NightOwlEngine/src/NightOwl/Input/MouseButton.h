#pragma once

#include "GLFW/glfw3.h"

namespace NightOwl
{
	enum class MouseButton
	{
		LeftMouseButton,
		RightMouseButton,
		MiddleMouseButton,
		None,
		NumberOfSupportedMouseButtons
	};

	inline int NightOwlMouseButtonToGlfwMouseButton(MouseButton mouseButton)
	{
		int glfwMouseButton;

		switch (mouseButton)
		{
		case MouseButton::LeftMouseButton:
		case MouseButton::RightMouseButton:
		case MouseButton::MiddleMouseButton:
			glfwMouseButton = static_cast<int>(mouseButton) + GLFW_MOUSE_BUTTON_LEFT - static_cast<int>(MouseButton::LeftMouseButton);
			break;

		default:
			glfwMouseButton = -1;
		}

		return glfwMouseButton;
	}

	inline MouseButton GlfwMouseButtonToNightOwlMouseButton(int glfwMouseButton)
	{
		MouseButton nightOwlMouseButton;

		switch (glfwMouseButton)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		case GLFW_MOUSE_BUTTON_RIGHT:
		case GLFW_MOUSE_BUTTON_MIDDLE:
			nightOwlMouseButton = static_cast<MouseButton>(static_cast<int>(MouseButton::LeftMouseButton) + glfwMouseButton - GLFW_MOUSE_BUTTON_LEFT);
			break;

		default:
			nightOwlMouseButton = MouseButton::None;
		}

		return nightOwlMouseButton;
	}
}
