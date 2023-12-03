#pragma once

#include "NightOwl/Math/Mat4.h"
#include "NightOwl/Math/Quaternion.h"

namespace NightOwl
{
	class BoneInfo
	{
	public:
		int id;

		Mat4F offsetMatrix;

		QuatF offsetRotation;
	};
}
