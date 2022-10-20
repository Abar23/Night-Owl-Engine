#pragma once

#include "WindowProperties.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include "GLFW/glfw3.h"
#include <functional>

namespace NightOwl::Window
{
	class GlfwWindow : public IWindow
	{
	public:
		GlfwWindow(const std::string& windowName, const unsigned int height, const unsigned int width);

		~GlfwWindow() override;

		bool ShouldWindowClose() override;

		unsigned GetHeight() override;

		unsigned GetWidth() override;

		void* GetWindowHandle() override;

		void Update() override;

		float GetAspectRatio() override;

		const WindowProperties& GetWindowProperties();

		void SetKeyCallback(std::function<void(int, int, int, int)> keyCallback) override;

		void SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback) override;

		void SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback) override;

		void SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback) override;

	private:
		GLFWwindow* windowHandle;

		WindowProperties properties;

		static void ErrorCallback(int errorCode, const char* errorMessage);

		static void ScreenResizeCallback(GLFWwindow* window, int width, int height);

		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static void MousePositionCallback(GLFWwindow* window, double xPosition, double yPosition);

		static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	};
}
