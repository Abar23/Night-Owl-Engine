#pragma once

#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class ClothParticle
	{
	public:
		Vec3F totalForce;

		Vec3F* position;

		Vec3F previousPosition;

		bool isFixed;
	};
}
