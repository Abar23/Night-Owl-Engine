#pragma once

#include "WindowProperties.h"
#include "NightOwl/Window/Interfaces/IWindow.h"
#include "NightOwl/Graphics/Interfaces/IContext.h"
#include "GLFW/glfw3.h"
#include <memory>

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

		std::unique_ptr<Graphics::IContext> graphicsContext;

		static void ErrorCallback(int errorCode, const char* errorMessage);

		static void ResizeCallback(GLFWwindow* window, int width, int height);
	};
}
