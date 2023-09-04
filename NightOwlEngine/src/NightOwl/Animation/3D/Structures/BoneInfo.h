#pragma once

#include "NightOwl/Math/Mat4.h"

namespace NightOwl
{
	class BoneInfo
	{
	public:
		int id;

		Mat4F offsetMatrix;
	};
}
