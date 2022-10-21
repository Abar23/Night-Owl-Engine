#pragma once

#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Input/KeyCode.h"
#include "NightOwl/Input/InputAction.h"
#include "NightOwl/Input/MouseButton.h"
#include <mutex>

namespace NightOwl::Input
{
	class Input
	{
	public:
		Input(const Input& other) = delete;

		void operator=(const Input& other) = delete;

		static void Update();

		static Input* GetInstance();

		static void Init();

		static const Math::Vec2D& GetScrollDelta();

		static bool IsKeyPressed(KeyCode nightOwlKeyCode);

		static bool IsKeyRelease(KeyCode nightOwlKeyCode);

		static bool IsKeyHeld(KeyCode nightOwlKeyCode);

		static bool IsMouseButtonPressed(KeyCode nightOwlMouseButton);

		static bool IsMouseButtonRelease(KeyCode nightOwlMouseButton);

		static bool IsMouseButtonHeld(KeyCode nightOwlMouseButton);

		static bool IsAnyKeyHeld();

		static bool IsAnyKeyPressed();

	private:
		Input();

		static Input* inputInstance;

		static std::mutex mutexLock;

		Math::Vec2D scrollDelta;

		Math::Vec2D mousePosition;

		Math::Vec2D mousePositionDelta;

		std::array<InputAction, static_cast<int>(KeyCode::NumberOfSupportedKeys)> keyActionArray;

		std::array<InputAction, static_cast<int>(MouseButton::NumberOfSupportedMouseButtons)> mouseButtonActionArray;

		unsigned int anyKeyHeldCounter;

		unsigned int anyKeyPressedCounter;

		bool isKeyInputDirty;

		bool isMouseButtonInputDirty;

		static void KeyCallback(int key, int scancode, int action, int mods);

		static void MouseButtonCallback(int button, int action, int mods);

		static void MousePositionCallback(double xPosition, double yPosition);

		static void MouseScrollCallback(double xOffset, double yOffset);
	};
}
