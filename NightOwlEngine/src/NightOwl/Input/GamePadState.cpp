#include <NightOwlPch.h>

#include "GamePadState.h"
#include "GamePadAxis.h"
#include "NightOwl/Math/MathFunctions.h"

namespace NightOwl
{
	GamePadState::GamePadState()
		:	buttons{ InputAction::None },
			axes{ 0.0f },
			gamePadConnection(GamePadConnection::Disconnected)
	{ }

	bool GamePadState::IsGamePadConnected()
	{
		return gamePadConnection == GamePadConnection::Connected;
	}

	void GamePadState::SetGamePadConnectionStatus(GamePadConnection status)
	{
		gamePadConnection = status;
	}

	std::array<InputAction, static_cast<int>(GamePadButton::NumberOfSupportedGamePadButtons)>& GamePadState::GetButtons()
	{
		return buttons;
	}

	void GamePadState::SetButton(GamePadButton button, InputAction inputAction)
	{
		buttons[static_cast<int>(button)] = inputAction;
	}

	InputAction GamePadState::GetButton(GamePadButton button)
	{
		return buttons[static_cast<int>(button)];
	}

	std::array<float, 6>& GamePadState::GetAxes()
	{
		return axes;
	}

	void GamePadState::SetAxes(const std::array<float, 6>& axes)
	{
		for (int axisIndex = 0; axisIndex < this->axes.size(); axisIndex++)
		{
			this->axes[axisIndex] = axes[axisIndex];
		}

		NormalizeTriggers();
	}

	void GamePadState::NormalizeTriggers()
	{
		float& leftTriggerAxisValue = axes[static_cast<int>(GamePadAxis::LeftTrigger)];
		leftTriggerAxisValue = MapValueInRange(leftTriggerAxisValue, -1.0f, 1.0f, 0.0f, 1.0f);

		float& rightTriggerAxisValue = axes[static_cast<int>(GamePadAxis::RightTrigger)];
		rightTriggerAxisValue = MapValueInRange(rightTriggerAxisValue, -1.0f, 1.0f, 0.0f, 1.0f);
	}
}
