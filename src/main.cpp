#include <iostream>
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"

int main()
{
	NightOwl::Math::Vec3F a(1, 2, 3);
	NightOwl::Utility::LoggerManager::Init();
	ENGINE_LOG_ERROR("I am the engine logger!");
	ENGINE_LOG_INFO("I am the engine logger!");
	CLIENT_LOG_ERROR("I am the client logger!");
	CLIENT_LOG_INFO("I am the client logger!");
	std::cout << a;
	std::cout << "Hi" << std::endl;
	return 0;
}
