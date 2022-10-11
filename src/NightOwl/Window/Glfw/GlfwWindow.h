#pragma once

#include "WindowProperties.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include "GLFW/glfw3.h"

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

	private:
		GLFWwindow* windowHandle;

		WindowProperties properties;

		static void ErrorCallback(int errorCode, const char* errorMessage);

		static void ResizeCallback(GLFWwindow* window, int width, int height);
	};
}
