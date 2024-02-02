#pragma once

#include "NightOwl/Math/Vec3.h"
#include "NightOwl/Math/Vec4.h"

namespace NightOwl
{
	class LightGraphicsStruct
	{
	public:
		Vec3F position;
		int padding;

		Vec3F color;
		float padding0;
	};
}
