#pragma once

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

		float far;

		float near;
	};
}
