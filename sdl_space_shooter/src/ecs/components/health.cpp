#include "pch.h"

#include "health.h"

namespace ecs
{
	namespace components
	{
		health::health()
			: max_health(0), current_health(0) {}
		health::health(int max_health)
			: max_health(max_health), current_health(max_health) {}
	}
}