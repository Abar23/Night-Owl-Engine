#pragma once

#include "GLFW/glfw3.h"
#include <string>

namespace NightOwl
{
	enum class GamePadConnection
	{
		Connected,
		Disconnected
	};

	inline int NightOwlGamePadConnectionToGlfwGamePadConnection(GamePadConnection gamePadConnection) 
	{

		int GlfwGamePadConnection;

		switch (gamePadConnection)
		{
		case GamePadConnection::Connected:
			GlfwGamePadConnection = GLFW_CONNECTED;
			break;

		case GamePadConnection::Disconnected:
			GlfwGamePadConnection = GLFW_DISCONNECTED;
			break;

		default:
			GlfwGamePadConnection = -1;
			break;
		}

		return GlfwGamePadConnection;
	}

	inline GamePadConnection GlfwGamePadConnectionToNightOwlGamePadConnection(int glfwgamePadConnection)
	{
		GamePadConnection nightOwlGamePadConnection;

		switch (glfwgamePadConnection)
		{
		case GLFW_CONNECTED:
			nightOwlGamePadConnection = GamePadConnection::Connected;
			break;

		case GLFW_DISCONNECTED:
		default:
			nightOwlGamePadConnection = GamePadConnection::Disconnected;
			break;
		}

		return nightOwlGamePadConnection;
	}

	inline std::string ToString(GamePadConnection gamePadConnection) {
		
		if (gamePadConnection == GamePadConnection::Connected) 
		{
			return "GamePadConnected";
		}

		return "GamePadDisconnected";
	}
}