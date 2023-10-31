#pragma once

namespace NightOwl
{
	template <typename T>
	bool NearEquals(const T floatOne, const T floatTwo);

	template <typename T>
	T DegreesToRad(const T angleInDegrees);

	template <typename T>
	T RadToDegrees(const T angleInDegrees);

	template <typename T>
	T FastInverseSquareRootAroundOne(const T value);

	template <typename T>
	T MapValueInRange(const T value, const T inputStart, const T inputEnd, const T outputStart, const T outputEnd);

	template <typename T>
	T Lerp(const T startValue, const T endValue, const T t);

	float SinEaseInEaseOutWithConstantVelocity(float time, float totalTime, float timeOne, float timeTwo);
}

#include "MathFunctions.inl"
