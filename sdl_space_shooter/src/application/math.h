#pragma once
#include <pch.h>

#include <random>
#include <type_traits>

namespace math
{
	inline float magnitude(const float x, const float y)
	{
		const auto magnitude = pow(x * x + y * y, 0.5f);
		return magnitude == 0.f ? 1.f : magnitude;
	}
}
