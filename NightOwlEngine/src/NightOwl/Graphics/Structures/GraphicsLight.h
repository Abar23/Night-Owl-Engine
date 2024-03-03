#pragma once

#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class Light;

	class GraphicsPointLight
	{
	public:
		GraphicsPointLight(const Light* light);

		GraphicsPointLight& operator=(const Light* light);
		
		Vec3F position;

		float padding0;

		Vec3F color;

		float range;

		float intensity;

		float shadowBias;

		float padding1;
		float padding2;
	};

	class GraphicsDirectionalLight
	{
	public:
		GraphicsDirectionalLight(const Light* light);

		GraphicsDirectionalLight& operator=(const Light* light);

		Vec3F direction;
		float padding0;

		Vec3F color;

		float intensity;

		float shadowBias;

		Vec3F padding;
	};
}
