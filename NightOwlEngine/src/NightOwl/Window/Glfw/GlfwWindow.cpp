#include <NightOwlPch.h>

#include "GlfwWindow.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/Structures/MonitorProperties.h"
#include "NightOwl/Window/Structures/WindowProperties.h"
#include <GLFW/glfw3.h>

namespace NightOwl
{
	GlfwWindow::GlfwWindow(const std::string& windowName, const unsigned int height, const unsigned int width)
		:	isFullScreen(false)
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
		
		RenderApi::CreateContext(this);//Create context

		glfwGetFramebufferSize(windowHandle, &windowProperties.pixelWidth, &windowProperties.pixelHeight);

		windowProperties.hasAspectRatioChanged = false;
		windowProperties.isMinimized = false;
		windowProperties.screenWidth = width;
		windowProperties.screenHeight = height;
		windowProperties.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		windowProperties.windowName = windowName;

		glfwSetWindowUserPointer(windowHandle, &windowProperties);
		
		monitorHandle = glfwGetPrimaryMonitor();
		
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitorHandle);
		
		monitorProperties.monitorWidth = videoMode->width;
		monitorProperties.monitorHeight = videoMode->height;
		monitorProperties.refreshRate = videoMode->refreshRate;
		
		glfwSetMonitorUserPointer(monitorHandle, &monitorProperties);
		
		glfwSwapInterval(0);

		glfwSetWindowSizeCallback(windowHandle, ScreenResizeCallback);
		glfwSetFramebufferSizeCallback(windowHandle, FramebufferResizeCallback);
		glfwSetKeyCallback(windowHandle, KeyCallback);
		glfwSetCursorPosCallback(windowHandle, MousePositionCallback);
		glfwSetMouseButtonCallback(windowHandle, MouseButtonCallback);
		glfwSetScrollCallback(windowHandle, MouseScrollCallback);
		glfwSetJoystickCallback(GamepadConnectionCallback);
		glfwSetWindowIconifyCallback(windowHandle, WindowMinimizationCallback);
	}

	void GlfwWindow::Shutdown()
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
		return windowProperties.screenHeight;
	}

	unsigned GlfwWindow::GetWidth()
	{
		return windowProperties.screenWidth;
	}

	void* GlfwWindow::GetWindowHandle()
	{
		return windowHandle;
	}

	GamePadConnection GlfwWindow::GetGamePadConnectionStatus(GamePadId gamePadId)
	{
		int isConnected = glfwJoystickIsGamepad(NightOwlGamePadIdToGlfwGamePadId(gamePadId));

		if (isConnected)
		{
			return GamePadConnection::Connected;
		}

		return GamePadConnection::Disconnected;
	}

	GamePadState GlfwWindow::GetGamePadState(GamePadId gamePadId)
	{
		GamePadState gamePadState;
		GLFWgamepadstate glfwGamePadState;

		glfwGetGamepadState(static_cast<int>(gamePadId), &glfwGamePadState);

		auto& buttons = gamePadState.GetButtons();
		auto& axes = gamePadState.GetAxes();

		for (int buttonIndex = 0; buttonIndex < buttons.size(); buttonIndex++)
		{
			buttons[buttonIndex] = GlfwInputActionToNightOwlKeyAction(glfwGamePadState.buttons[buttonIndex]);
		}

		for (int axisIndex = 0; axisIndex < axes.size(); axisIndex ++)
		{
			axes[axisIndex] = glfwGamePadState.axes[axisIndex];
		}

		return gamePadState;
	}

	float GlfwWindow::GetAspectRatio()
	{
		return windowProperties.aspectRatio;
	}

	const WindowProperties& GlfwWindow::GetWindowProperties()
	{
		return windowProperties;
	}

	const MonitorProperties& GlfwWindow::GetMonitorProperties()
	{
		return monitorProperties;
	}

	void GlfwWindow::SetKeyCallback(std::function<void(int, int, int, int)> keyCallback)
	{
		windowProperties.keyCallback = keyCallback;
	}

	void GlfwWindow::SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback)
	{
		windowProperties.mouseButtonCallback = mouseButtonCallback;
	}

	void GlfwWindow::SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback)
	{
		windowProperties.mousePositionCallback = mousePositionCallback;
	}

	void GlfwWindow::SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback)
	{
		windowProperties.mouseScrollCallback = mouseScrollCallback;
	}

	void GlfwWindow::SetGamePadConnectionCallback(std::function<void(int, int)> gamepadConnectionCallback)
	{
		this->gamepadConnectionCallback = gamepadConnectionCallback;
	}

	void GlfwWindow::ToggleFullScreen()
	{
		if (glfwGetWindowMonitor(windowHandle) != nullptr || monitorHandle == nullptr)
		{
			glfwSetWindowMonitor(windowHandle,
								 nullptr,
								 monitorProperties.windowSnapshot.positionX,
								 monitorProperties.windowSnapshot.positionY,
								 monitorProperties.windowSnapshot.width,
								 monitorProperties.windowSnapshot.height,
								 GLFW_DONT_CARE);

			isFullScreen = false;

			return;
		}

		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(windowHandle));
		glfwGetWindowPos(windowHandle, &monitorProperties.windowSnapshot.positionX, &monitorProperties.windowSnapshot.positionY);
		monitorProperties.windowSnapshot.width = properties->screenWidth;
		monitorProperties.windowSnapshot.height = properties->screenHeight;

		const GLFWvidmode* videoMode = glfwGetVideoMode(monitorHandle);
		monitorProperties.monitorWidth = videoMode->width;
		monitorProperties.monitorHeight = videoMode->height;
		monitorProperties.refreshRate = videoMode->refreshRate;

		glfwSetWindowMonitor(windowHandle,
							 monitorHandle,
							 0,
							 0,
							 monitorProperties.monitorWidth,
							 monitorProperties.monitorHeight,
							 monitorProperties.refreshRate);

		isFullScreen = true;
	}

	bool GlfwWindow::IsFullScreen()
	{
		return isFullScreen;
	}

	bool GlfwWindow::IsMinimized()
	{
		return windowProperties.isMinimized;
	}

	void GlfwWindow::SetMonitor(uint32_t index)
	{
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		ENGINE_ASSERT(index >= count, "Cannot Set Monitor at index: {0}", index);

		monitorHandle = monitors[index];
		
		const WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(windowHandle));
		glfwGetWindowPos(windowHandle, &monitorProperties.windowSnapshot.positionX, &monitorProperties.windowSnapshot.positionY);
		monitorProperties.windowSnapshot.width = properties->screenWidth;
		monitorProperties.windowSnapshot.height = properties->screenHeight;

		const GLFWvidmode* videoMode = glfwGetVideoMode(monitorHandle);
		monitorProperties.monitorWidth = videoMode->width;
		monitorProperties.monitorHeight = videoMode->height;
		monitorProperties.refreshRate = videoMode->refreshRate;

		glfwSetWindowMonitor(windowHandle, 
							 monitorHandle, 
							 0, 
							 0, 
							 monitorProperties.monitorWidth, 
							 monitorProperties.monitorHeight, 
							 monitorProperties.refreshRate);
	}

	int GlfwWindow::GetTotalMonitors()
	{
		int count;
		glfwGetMonitors(&count);
		return count;
	}

	void GlfwWindow::CloseWindow()
	{
		glfwSetWindowShouldClose(windowHandle, true);
	}

	void GlfwWindow::EnableCursor()
	{
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void GlfwWindow::HideCursor()
	{
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void GlfwWindow::DisableCursor()
	{
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	bool GlfwWindow::HasWindowChangedAspectRatio()
	{
		return windowProperties.hasAspectRatioChanged;
	}

	void GlfwWindow::Update()
	{
		windowProperties.hasAspectRatioChanged = false;

		glfwPollEvents();

		RenderApi::GetContext()->SwapBuffers();
	}

	void GlfwWindow::ErrorCallback(int errorCode, const char* errorMessage)
	{
		ENGINE_LOG_ERROR("GFLW error callback triggered. Error({0}): {1}", errorCode, errorMessage);
	}

	void GlfwWindow::ScreenResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->screenHeight = height;
		properties->screenWidth = width;
		properties->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		properties->hasAspectRatioChanged = true;
	}

	void GlfwWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->pixelHeight = height;
		properties->pixelWidth = width;

		RenderApi::GetContext()->SetViewport(0, 0, width, height);
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

	void GlfwWindow::GamepadConnectionCallback(int gamepadId, int connectionEventType)
	{
		if (gamepadConnectionCallback != nullptr)
		{
			gamepadConnectionCallback(gamepadId, connectionEventType);
		}
	}

	void GlfwWindow::WindowMinimizationCallback(GLFWwindow* window, int iconified)
	{
		WindowProperties* properties = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		properties->isMinimized = static_cast<bool>(iconified);
	}
}
