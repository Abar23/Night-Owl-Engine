#pragma once

#include <string>
#include <functional>

class WindowProperties
{
public:
	int pixelHeight;

	int pixelWidth;

	int screenHeight;

	int screnWidth;

	float aspectRatio;

	std::string windowName;

	std::function<void(int, int, int, int)> keyCallback;

	std::function<void(int, int, int)> mouseButtonCallback;

	std::function<void(double, double)> mousePositionCallback;

	std::function<void(double, double)> mouseScrollCallback;
};
