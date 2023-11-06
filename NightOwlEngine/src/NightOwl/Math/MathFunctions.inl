#pragma once

#include "MathConstants.h"
#include <algorithm>

namespace NightOwl
{
	template <typename T>
	bool NearEquals(const T floatOne, const T floatTwo)
	{
		T absDiff = std::abs(floatOne - floatTwo);
		if (absDiff <= EPSILON)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	T DegreesToRad(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(FLOAT_PI / 180.0f);
	}

	template <typename T>
	T RadToDegrees(const T angleInDegrees)
	{
		return angleInDegrees * static_cast<T>(180.0f / FLOAT_PI);
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

	template <typename T>
	T Lerp(const T startValue, const T endValue, const T t)
	{
		T clampedT = std::clamp(t, static_cast<T>(0), static_cast<T>(1));

		return (static_cast<T>(1) - clampedT) * startValue + clampedT * endValue;
	}

	inline float SinEaseInEaseOutWithConstantVelocity(float time, float totalTime, float timeOne, float timeTwo)
	{
		const float clampedTime = std::clamp(time, 0.0f, totalTime);
		const float inverseTotalTime = 1.0f / totalTime;
		const float normalizedTimeOne = timeOne * inverseTotalTime;
		const float normalizedTimeTwo = timeTwo * inverseTotalTime;
		const float normalizedTime = clampedTime * inverseTotalTime;

		const float inverseL = 1.0f / ((FLOAT_PI / 2.0f - 1.0f) * (normalizedTimeTwo - normalizedTimeOne) + 1.0f);

		float distance;

		if (normalizedTime <= normalizedTimeOne)
		{
			const float normalizedTimeOneOverL = normalizedTimeOne * inverseL;
			distance = normalizedTimeOneOverL * std::sinf(((normalizedTime - normalizedTimeOne) / normalizedTimeOne) * FLOAT_PI / 2.0f) + normalizedTimeOneOverL;
		}
		else if (normalizedTime > normalizedTimeOne && normalizedTime < normalizedTimeTwo)
		{
			distance = (FLOAT_PI * 0.5f * inverseL) * (normalizedTime - normalizedTimeOne) + (normalizedTimeOne * inverseL);
		}
		else
		{
			constexpr float piOverTwo = FLOAT_PI / 2.0f;

			const float leftTerm = (1.0f - normalizedTimeTwo) * inverseL * std::sinf(piOverTwo * ((normalizedTime - normalizedTimeTwo) / (1.0f - normalizedTimeTwo)));
			const float middleTerm = (normalizedTimeTwo - normalizedTimeOne) * piOverTwo * inverseL;
			const float rightTerm = normalizedTimeOne * inverseL;

			distance = leftTerm + middleTerm + rightTerm;
		}

		return distance;
	}
}
