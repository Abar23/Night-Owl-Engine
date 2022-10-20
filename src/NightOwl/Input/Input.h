#pragma once

#include "NightOwl/Math/Vec2.h"

namespace NightOwl::Input
{
	class Input
	{
	public:
		Input();

		void Update();

		const Math::Vec2D& GetScrollDelta() const;

	private:
		Math::Vec2D scrollDelta;

		void keyCallback(int key, int scancode, int action, int mods);

		void MouseButtonCallback(int button, int action, int mods);

		void MousePositionCallback(double xPosition, double yPosition);

		void MouseScrollCallback(double xOffset, double yOffset);
	};
}
