#pragma once

#include "InputAction.h"
#include "GamePadConnection.h"
#include "GamePadButton.h"
#include <array>

namespace NightOwl
{
	class GamePadState
	{
	public:
		GamePadState();
	
		bool IsGamePadConnected();

		void SetGamePadConnectionStatus(GamePadConnection status);

		std::array<InputAction, static_cast<int>(GamePadButton::NumberOfSupportedGamePadButtons)>& GetButtons();
		
		void SetButton(GamePadButton button, InputAction inputAction);

		InputAction GetButton(GamePadButton button);

		std::array<float, 6>& GetAxes();

		void SetAxes(const std::array<float, 6>& axes);
	
	private:
		std::array<InputAction, static_cast<int>(GamePadButton::NumberOfSupportedGamePadButtons)> buttons;

		std::array<float, 6> axes;

		GamePadConnection gamePadConnection;

		void NormalizeTriggers();
	};
}
