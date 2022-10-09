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
		graphicsContext = Graphics::RenderApi::GetInstance()->CreateContext(this);//Create context

		properties.width = width;
		properties.height = height;
		properties.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		properties.windowName = windowName;

		glfwSetWindowUserPointer(windowHandle, &properties);
		glfwSwapInterval(1);

		glfwSetWindowSizeCallback(windowHandle, ResizeCallback);
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
		return properties.height;
	}

	unsigned GlfwWindow::GetWidth()
	{
		return properties.width;
	}

	void* GlfwWindow::GetWindowHandle()
	{
		return windowHandle;
	}

	float GlfwWindow::GetAspectRatio()
	{
		return properties.aspectRatio;
	}

	void GlfwWindow::Update()
	{
		glfwPollEvents();
		graphicsContext->SwapBuffers();
	}

	void GlfwWindow::ErrorCallback(int errorCode, const char* errorMessage)
	{
		ENGINE_LOG_ERROR("GFLW error callback triggered. Error({0}): {1}", errorCode, errorMessage);
	}

	void GlfwWindow::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->height = height;
		properties->width = width;
		properties->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
}
