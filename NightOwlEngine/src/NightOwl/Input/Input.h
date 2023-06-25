#pragma once

#include "NightOwl/Math/Vec2.h"
#include "KeyCode.h"
#include "InputAction.h"
#include "MouseButton.h"
#include "GamePadState.h"
#include "GamePadId.h"
#include "GamePadAxis.h"
#include <mutex>
#include <memory>

namespace NightOwl
{
	class Input
	{
	public:
		Input(const Input& other) = delete;

		void operator=(const Input& other) = delete;

		static void Update();

		static void Init();

		static const Vec2D& GetScrollDelta();

		static bool IsKeyPressed(KeyCode nightOwlKeyCode);

		static bool IsKeyRelease(KeyCode nightOwlKeyCode);

		static bool IsKeyHeld(KeyCode nightOwlKeyCode);

		static bool IsMouseButtonPressed(KeyCode nightOwlMouseButton);

		static bool IsMouseButtonRelease(KeyCode nightOwlMouseButton);

		static bool IsMouseButtonHeld(KeyCode nightOwlMouseButton);

		static bool IsGamePadButtonPressed(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton);

		static bool HasAnyGamePadPressedButton(GamePadButton nightOwlGamePadButton);

		static bool IsGamePadButtonRelease(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton);

		static bool HasAnyGamePadReleasedButton(GamePadButton nightOwlGamePadButton);

		static bool IsGamePadButtonHeld(GamePadId nightOwlGamePadId, GamePadButton nightOwlGamePadButton);

		static bool HasAnyGamePadHeldButton(GamePadButton nightOwlGamePadButton);

		static bool IsGamePadConnected(GamePadId nightOwlGamePadId);

		static float GetGamePadAxis(GamePadId nightOwlGamePadId, GamePadAxis nightOwlGamePadAxis);

		static std::array<float, 4> GetAxisOfAllGamePads(GamePadAxis nightOwlGamePadAxis);

		static bool IsAnyKeyHeld();

		static bool IsAnyKeyPressed();

	private:
		Input();

		static Input* GetInstance();

		static std::unique_ptr<Input> inputInstance;

		static std::mutex mutexLock;

		Vec2D scrollDelta;

		Vec2D mousePosition;

		Vec2D mousePositionDelta;

		std::array<InputAction, static_cast<int>(KeyCode::NumberOfSupportedKeys)> keyActionArray;

		std::array<InputAction, static_cast<int>(MouseButton::NumberOfSupportedMouseButtons)> mouseButtonActionArray;

		std::array<GamePadState, 4> gamePads;

		unsigned int anyKeyHeldCounter;

		unsigned int anyKeyPressedCounter;

		bool isKeyInputDirty;

		bool isMouseButtonInputDirty;

		static void UpdateKeys();

		static void UpdateMouseButtons();

		static void UpdateGamePads();

		static void KeyCallback(int key, int scancode, int action, int mods);

		static void MouseButtonCallback(int button, int action, int mods);

		static void MousePositionCallback(double xPosition, double yPosition);

		static void MouseScrollCallback(double xOffset, double yOffset);

		static void GamePadConnectionCallback(int gamePadId, int connectionEventType);

		static bool DoesAnyGameButtonHaveState(GamePadButton button, InputAction action);

		static void CheckForConnectedControllers();

		static void SetupWindowCallbacks();
	};
}
