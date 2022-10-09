#include <iostream>

#include "GLFW/glfw3.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/Glfw/GlfwWindow.h"

int main()
{
	NightOwl::Math::Vec3F a(1, 2, 3);
	NightOwl::Utility::LoggerManager::Init();
	ENGINE_LOG_ERROR("I am the engine logger!");
	ENGINE_LOG_INFO("I am the engine logger!");
	CLIENT_LOG_ERROR("I am the client logger!");
	CLIENT_LOG_INFO("I am the client logger!");
	CLIENT_LOG_ASSERT(std::string("HI"));
	std::cout << a;
	std::cout << "Hi" << std::endl;

	NightOwl::Window::IWindow* window = new NightOwl::Window::GlfwWindow("Hi", 200, 200);
	while(!window->ShouldWindowClose())
	{
		window->Update();
	}
		
	return 0;
}
