#pragma once

#include <functional>
#include "NightOwl/Input/GamePadState.h"
#include "NightOwl/Input/GamePadId.h"

namespace NightOwl
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual void Shutdown() = 0;

		virtual bool ShouldWindowClose() = 0;

		virtual unsigned int GetHeight() = 0;

		virtual unsigned int GetWidth() = 0;

		virtual void* GetWindowHandle() = 0;

		virtual float GetAspectRatio() = 0;

		virtual GamePadConnection GetGamePadConnectionStatus(GamePadId gamePadId) = 0;

		virtual GamePadState GetGamePadState(GamePadId GamePadId) = 0;

		virtual void Update() = 0;

		virtual void SetKeyCallback(std::function<void(int, int, int, int)> keyCallback) = 0;

		virtual void SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback) = 0;

		virtual void SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback) = 0;

		virtual void SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback) = 0;

		virtual void SetGamePadConnectionCallback(std::function<void(int, int)> gamepadConnectionCallback) = 0;

		virtual void ToggleFullScreen() = 0;

		virtual bool IsFullScreen() = 0;

		virtual bool IsMinimized() = 0;

		virtual void SetMonitor(uint32_t index) = 0;

		virtual int GetTotalMonitors() = 0;

		virtual void DisableCursor() = 0;

		virtual void HideCursor() = 0;

		virtual void CloseWindow() = 0;

		virtual void EnableCursor() = 0;

		virtual bool HasWindowChangedAspectRatio() = 0;
	};
}
