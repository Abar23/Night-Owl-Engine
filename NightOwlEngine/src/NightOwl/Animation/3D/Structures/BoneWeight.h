#pragma once

#include <array>

namespace NightOwl
{
	class BoneWeight
	{
	public:
		BoneWeight()
		{
			boneIds.fill(-1);
			weights.fill(0.0f);
		}

		std::array<int, 4> boneIds;

		std::array<float, 4> weights;
	};
}
