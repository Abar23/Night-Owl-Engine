#pragma once

#include <functional>

namespace NightOwl::Window
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual bool ShouldWindowClose() = 0;

		virtual unsigned int GetHeight() = 0;

		virtual unsigned int GetWidth() = 0;

		virtual void* GetWindowHandle() = 0;

		virtual float GetAspectRatio() = 0;

		virtual void Update() = 0;

		virtual void SetKeyCallback(std::function<void(int, int, int, int)> keyCallback) = 0;

		virtual void SetMouseButtonCallback(std::function<void(int, int, int)> mouseButtonCallback) = 0;

		virtual void SetMousePositionCallback(std::function<void(double, double)> mousePositionCallback) = 0;

		virtual void SetMouseScrollCallback(std::function<void(double, double)> mouseScrollCallback) = 0;
	};
}
