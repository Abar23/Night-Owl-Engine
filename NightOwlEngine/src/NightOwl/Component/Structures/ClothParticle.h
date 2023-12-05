#pragma once

#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class ClothParticle
	{
	public:
		Vec3F totalForce;

		int row;

		int column;

		Vec3F position;

		Vec3F previousPosition;

		bool isCorner;

		bool isFixed;
	};
}
