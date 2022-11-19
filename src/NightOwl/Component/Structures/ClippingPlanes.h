#pragma once

#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl::Component
{
	class ClippingPlanes
	{
	public:
		ClippingPlanes(float near, float far)
		{
			this->near = near;
			this->far = far;
		}

		REFLECT()

		float far;

		float near;
	};
}
