#include "pch.h"

#include "rotation.h"

namespace ecs
{
	namespace components
	{
		rotation::rotation()
			: angle(0) {}
		rotation::rotation(const double angle)
			: angle(angle) {}
	}
}
