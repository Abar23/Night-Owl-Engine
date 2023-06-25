#pragma once

#include <string>
#include <functional>

namespace NightOwl
{
	class WindowProperties
	{
	public:
		int pixelHeight;

		int pixelWidth;

		int screenHeight;

		int screenWidth;

		float aspectRatio;

		std::string windowName;

		bool isMinimized;

		bool hasAspectRatioChanged;

		std::function<void(int, int, int, int)> keyCallback;

		std::function<void(int, int, int)> mouseButtonCallback;

		std::function<void(double, double)> mousePositionCallback;

		std::function<void(double, double)> mouseScrollCallback;
	};
}
