#include "Input.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl::Input
{
	using namespace std::placeholders;

	Input::Input()
	{
		Window::WindowApi::GetWindow()->SetKeyCallback(std::bind(&Input::keyCallback, this, _1, _2, _3, _4));
		Window::WindowApi::GetWindow()->SetMouseButtonCallback(std::bind(&Input::MouseButtonCallback, this, _1, _2, _3));
		Window::WindowApi::GetWindow()->SetMousePositionCallback(std::bind(&Input::MousePositionCallback, this, _1, _2));
		Window::WindowApi::GetWindow()->SetMouseScrollCallback(std::bind(&Input::MouseScrollCallback, this, _1, _2));
	}

	void Input::Update()
	{
		scrollDelta.x = 0;
		scrollDelta.y = 0;
	}

	const Math::Vec2D& Input::GetScrollDelta() const
	{
		return scrollDelta;
	}

	void Input::keyCallback(int key, int scancode, int action, int mods)
	{
		ENGINE_LOG_INFO("Key {0} Scancode {1} Action {2} Mods {3}", key, scancode, action, mods);
	}

	void Input::MouseButtonCallback(int button, int action, int mods)
	{
		ENGINE_LOG_INFO("Button {0} Action {1} Mods {2}", button, action, mods);
	}

	void Input::MousePositionCallback(double xPosition, double yPosition)
	{
		ENGINE_LOG_INFO("xPosition {0} yPosition {1}", xPosition, yPosition);
	}

	void Input::MouseScrollCallback(double xOffset, double yOffset)
	{
		ENGINE_LOG_INFO("xOffset {0} yOffset {1}", xOffset, yOffset);

		scrollDelta.x = xOffset;
		scrollDelta.y = yOffset;
	}
}
