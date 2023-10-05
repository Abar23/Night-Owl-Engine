#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace NightOwl
{
	enum class GamePadId
	{
		Pad1,
		Pad2,
		Pad3,
		Pad4,
		None
	};

	inline int NightOwlGamePadIdToGlfwGamePadId(GamePadId gamePadId)
	{
		int glfwGamePad;

		switch (gamePadId)
		{
		case GamePadId::Pad1:
		case GamePadId::Pad2:
		case GamePadId::Pad3:
		case GamePadId::Pad4:
			glfwGamePad = static_cast<int>(gamePadId) + GLFW_JOYSTICK_1 - static_cast<int>(GamePadId::Pad1);
			break;

		default:
			glfwGamePad = -1;
			break;
		}
		return glfwGamePad;
	}

	inline GamePadId GlfwGamePadIdToNightOwlGamePadId(int glfwGamePad)
	{
		GamePadId nightOwlGamePad;

		switch (glfwGamePad)
		{
		case GLFW_JOYSTICK_1:
		case GLFW_JOYSTICK_2:
		case GLFW_JOYSTICK_3:
		case GLFW_JOYSTICK_4:
			nightOwlGamePad = static_cast<GamePadId>(static_cast<int>(GamePadId::Pad1) + glfwGamePad - GLFW_JOYSTICK_1);
			break;

		default:
			nightOwlGamePad = GamePadId::None;
			break;
		}
		return nightOwlGamePad;
	}

	inline std::string ToString(GamePadId gamePadId) 
	{
		switch (gamePadId)
		{
		case GamePadId::Pad1:
			return "GamePad1";

		case GamePadId::Pad2:
			return "GamePad2";

		case GamePadId::Pad3:
			return "GamePad3";

		case GamePadId::Pad4:
			return "GamePad4";

		default:
			return "None";
		}
	}
}
