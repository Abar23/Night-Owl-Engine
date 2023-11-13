#pragma once

#include <cmath>

namespace NightOwl
{
	constexpr float EPSILON = 1.0e-4f;

	constexpr float FLOAT_PI = static_cast<float>(M_PI);

	constexpr float SLERP_DOT_THRESHOLD = 0.9f;

	constexpr float THREE_HUNDRED_SIXTY_DEGREES = 360.0f;

	constexpr float NORTH_POLE_SINGULARITY_VALUE = 0.499f;

	constexpr float SOUTH_POLE_SINGULARITY_VALUE = -0.499f;
}
