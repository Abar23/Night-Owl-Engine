#include "GlfwWindow.h"
#include "GLFW/glfw3.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"

namespace NightOwl::Window
{
	GlfwWindow::GlfwWindow(const std::string& windowName, const unsigned int height, const unsigned int width)
	{
		int glfwStartedSuccessfully = glfwInit();

		ENGINE_ASSERT(glfwStartedSuccessfully, "GLFW failed to Initialize!");

		#ifdef DEBUG
		glfwSetErrorCallback(ErrorCallback);
			#ifdef OPEN_GL
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif
		#endif

		windowHandle = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), windowName.c_str(), nullptr, nullptr);
		Graphics::RenderApi::CreateContext(this);//Create context

		glfwGetFramebufferSize(windowHandle, &properties.pixelWidth, &properties.pixelHeight);
		properties.screnWidth = width;
		properties.screenHeight = height;
		properties.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		properties.windowName = windowName;

		glfwSetWindowUserPointer(windowHandle, &properties);
		glfwSwapInterval(1);

		glfwSetWindowSizeCallback(windowHandle, ScreenResizeCallback);
		glfwSetFramebufferSizeCallback(windowHandle, FramebufferResizeCallback);
		glfwSetKeyCallback(windowHandle, KeyCallback);
		glfwSetCursorPosCallback(windowHandle, MousePositionCallback);
		glfwSetMouseButtonCallback(windowHandle, MouseButtonCallback);
		glfwSetScrollCallback(windowHandle, MouseScrollCallback);
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
	}

	bool GlfwWindow::ShouldWindowClose()
	{
		return glfwWindowShouldClose(windowHandle);
	}

	unsigned GlfwWindow::GetHeight()
	{
		return properties.screenHeight;
	}

	unsigned GlfwWindow::GetWidth()
	{
		return properties.screnWidth;
	}

	void* GlfwWindow::GetWindowHandle()
	{
		return windowHandle;
	}

	float GlfwWindow::GetAspectRatio()
	{
		return properties.aspectRatio;
	}

	const WindowProperties& GlfwWindow::GetWindowProperties()
	{
		return properties;
	}

	void GlfwWindow::SetKeyCallback(std::function<void(int, int, int, int)> keyCallback)
	{
		properties.keyCallback = keyCallback;
	}

	void GlfwWindow::SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback)
	{
		properties.mouseButtonCallback = mouseButtonCallback;
	}

	void GlfwWindow::SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback)
	{
		properties.mousePositionCallback = mousePositionCallback;
	}

	void GlfwWindow::SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback)
	{
		properties.mouseScrollCallback = mouseScrollCallback;
	}

	void GlfwWindow::Update()
	{
		glfwPollEvents();
		Graphics::RenderApi::GetContext()->SwapBuffers();
	}

	void GlfwWindow::ErrorCallback(int errorCode, const char* errorMessage)
	{
		ENGINE_LOG_ERROR("GFLW error callback triggered. Error({0}): {1}", errorCode, errorMessage);
	}

	void GlfwWindow::ScreenResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->screenHeight = height;
		properties->screnWidth = width;
		properties->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	void GlfwWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->pixelHeight = height;
		properties->pixelWidth = width;

		Graphics::RenderApi::GetContext()->SetViewport(0, 0, width, height);
	}

	void GlfwWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		if(properties->keyCallback != nullptr)
		{
			properties->keyCallback(key, scancode, action, mods);
		}
	}

	void GlfwWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		if (properties->mouseButtonCallback != nullptr)
		{
			properties->mouseButtonCallback(button, action, mods);
		}
	}

	void GlfwWindow::MousePositionCallback(GLFWwindow* window, double xPosition, double yPosition)
	{
		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		if (properties->mousePositionCallback != nullptr)
		{
			properties->mousePositionCallback(xPosition, yPosition);
		}
	}

	void GlfwWindow::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		if (properties->mouseScrollCallback != nullptr)
		{
			properties->mouseScrollCallback(xOffset, yOffset);
		}
	}
}
