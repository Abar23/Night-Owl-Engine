#pragma once

#include "NightOwl/Window/Interfaces/IWindow.h"
#include "NightOwl/Window/Glfw/GlfwWindow.h"
#include <memory>

namespace NightOwl::Window
{
	class WindowApi
	{
	public:
		template<typename ...Args>
		static std::unique_ptr<IWindow> CreateWindow(Args&&... args)
		{
			return std::make_unique<GlfwWindow>(std::forward<Args>(args)...);
		}
	};
}
