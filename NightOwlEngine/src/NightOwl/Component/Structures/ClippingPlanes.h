#pragma once

#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl
{
	class ClippingPlanes
	{
	public:
		ClippingPlanes(float near, float far)
			:	far(far),
				near(near)
		{ }

		REFLECT()

		float far;

		float near;
	};
}
