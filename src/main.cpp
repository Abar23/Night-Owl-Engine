#include <iostream>
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logger.h"
int main()
{
	NightOwl::Math::Vec3F a(1, 2, 3);
	NightOwl::Utility::Logger l("./Log.txt");
	ENGINE_LOG_INFO("Hello!");
	std::cout << a;
	std::cout << "Hi" << std::endl;
	return 0;
}