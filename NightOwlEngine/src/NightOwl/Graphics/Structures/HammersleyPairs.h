#pragma once

#include "NightOwl/Math/Vec2.h"
#include <vector>

namespace NightOwl
{
	class HammersleyPairs
	{
	public:
		HammersleyPairs(int numberOfDesiredPairs);

		const std::vector<Vec2F>& GetHammersleyPairs();

	private:
		int numberOfDesiredPairs;

		std::vector<Vec2F> hammersleyPairs;

		void GeneratePairs();

		static Vec2F GeneratePoint(int pointIndex, int totalPoints);
	};
}
