#pragma once

#include "NightOwl/Input/GamePadState.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include "NightOwl/Window/Structures/MonitorProperties.h"
#include "NightOwl/Window/Structures/WindowProperties.h"
#include <functional>
#include <GLFW/glfw3.h>


namespace NightOwl
{
	class MonitorProperties;

	class WindowProperties;

	class GlfwWindow : public IWindow
	{
	public:
		GlfwWindow(const std::string& windowName, const unsigned int height, const unsigned int width);

		void Shutdown() override;

		bool ShouldWindowClose() override;

		unsigned GetHeight() override;

		unsigned GetWidth() override;

		void* GetWindowHandle() override;

		GamePadConnection GetGamePadConnectionStatus(GamePadId gamePadId) override;

		GamePadState GetGamePadState(GamePadId GamePadId) override;

		void Update() override;

		float GetAspectRatio() override;

		const WindowProperties& GetWindowProperties();

		const MonitorProperties& GetMonitorProperties();

		void SetKeyCallback(std::function<void(int, int, int, int)> keyCallback) override;

		void SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback) override;

		void SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback) override;

		void SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback) override;

		void SetGamePadConnectionCallback(std::function<void(int, int)> gamepadConnectionCallback) override;

		void ToggleFullScreen() override;

		bool IsFullScreen() override;

		bool IsMinimized() override;

		void SetMonitor(uint32_t index) override;

		int GetTotalMonitors() override;

		void CloseWindow() override;

		void EnableCursor() override;

		void HideCursor() override;

		void DisableCursor() override;

		bool HasWindowChangedAspectRatio() override;

	private:
		GLFWwindow* windowHandle;

		WindowProperties windowProperties;

		GLFWmonitor* monitorHandle;

		MonitorProperties monitorProperties;

		bool isFullScreen;

		inline static std::function<void(int, int)> gamepadConnectionCallback{nullptr};

		//* in callback function is used by GLFW may not need to be changed?
		static void ErrorCallback(int errorCode, const char* errorMessage);

		static void ScreenResizeCallback(GLFWwindow* window, int width, int height);

		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static void MousePositionCallback(GLFWwindow* window, double xPosition, double yPosition);

		static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

		static void GamepadConnectionCallback(int gamepadId, int connectionEventType);

		static void WindowMinimizationCallback(GLFWwindow* window, int iconified);
	};
}
