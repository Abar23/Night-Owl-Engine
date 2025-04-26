#include "NightOwlPch.h"

#include "HammersleyPairs.h"

namespace NightOwl
{
	HammersleyPairs::HammersleyPairs(int numberOfDesiredPairs)
		: numberOfDesiredPairs(numberOfDesiredPairs)
	{
		GeneratePairs();
	}

	const std::vector<Vec2F>& HammersleyPairs::GetHammersleyPairs()
	{
        return hammersleyPairs;
	}

	void HammersleyPairs::GeneratePairs()
	{
        for (int pointIndex = 0; pointIndex < numberOfDesiredPairs; ++pointIndex)
		{
			hammersleyPairs.push_back(GeneratePoint(pointIndex, numberOfDesiredPairs));
        }
	}

	Vec2F HammersleyPairs::GeneratePoint(int pointIndex, int totalPoints)
	{
        float accumulatedFraction = 0.0f;
        float normalizedIndex = (pointIndex + 0.5f) / totalPoints;

        for (float currentFraction = 0.5f, reversedIndex = pointIndex; reversedIndex; currentFraction *= 0.5f, reversedIndex /= 2) 
		{
            if (static_cast<int>(reversedIndex) & 1) 
			{
                accumulatedFraction += currentFraction;
            }
        }

        return { accumulatedFraction, normalizedIndex };
	}
}
