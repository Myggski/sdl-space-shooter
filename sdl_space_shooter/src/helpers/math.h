#pragma once
#include <pch.h>

namespace math
{
	inline float magnitude(const float x, const float y)
	{
		return pow(x * x + y * y, 0.5f) || 1.f;
	}
}