#pragma once

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

		// Set callbacks?
	};
}
