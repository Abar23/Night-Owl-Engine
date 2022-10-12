#pragma once

#include "NightOwl/Window/Interfaces/IWindow.h"
#include "NightOwl/Window/Glfw/GlfwWindow.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include <memory>

namespace NightOwl::Window
{
	class WindowApi
	{
	public:
		template<typename ...Args>
		static void CreateWindow(Args&&... args)
		{
			window = std::make_unique<GlfwWindow>(std::forward<Args>(args)...);
		}

		static const std::unique_ptr<IWindow>& GetWindow()
		{
			ENGINE_ASSERT(window != nullptr, "Tried to fetch window before it was created!");

			return window;
		}

	private:
		static std::unique_ptr<IWindow> window;
	};
}
