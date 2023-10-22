#pragma once

namespace NightOwl
{
	template <typename T>
	inline bool NearEquals(const T floatOne, const T floatTwo);

	template <typename T>
	inline T DegreesToRad(const T angleInDegrees);

	template <typename T>
	inline T RadToDegrees(const T angleInDegrees);
	template <typename T>
	inline T FastInverseSquareRootAroundOne(const T value);

	template <typename T>
	inline T MapValueInRange(const T value, const T inputStart, const T inputEnd, const T outputStart, const T outputEnd);
}

#include "MathFunctions.inl"
