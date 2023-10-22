#pragma once

#include "MathConstants.h"


namespace NightOwl
{
	template <typename T>
	bool NearEquals(const T floatOne, const T floatTwo)
	{
		T absDiff = std::abs(floatOne - floatTwo);
		if (absDiff <= SMALL_EPSILON)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	T DegreesToRad(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(F_PI / 180.0f);
	}

	template <typename T>
	T RadToDegrees(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(180.0f / F_PI);
	}

	template <typename T>
	T FastInverseSquareRootAroundOne(const T value)
	{
		constexpr T coeffOne = static_cast<T>(15.0f / 8.0f);
		constexpr T coeffTwo = static_cast<T>(-5.0f / 4.0f);
		constexpr T coeffThree = static_cast<T>(3.0f / 8.0f);

		return coeffOne + coeffTwo * value + coeffThree * value * value;
	}

	template <typename T>
	T MapValueInRange(const T value, const T inputStart, const T inputEnd, const T outputStart, const T outputEnd)
	{
		return outputStart + (outputEnd - outputStart) / (inputEnd - inputStart) * (value - inputStart);
	}
}
