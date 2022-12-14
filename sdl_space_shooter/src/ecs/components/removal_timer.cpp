#include "pch.h"

#include "removal_timer.h"

namespace ecs
{
	namespace components
	{
		removal_timer::removal_timer()
			: current_time(0) {}
		removal_timer::removal_timer(float current_time_seconds)
			: current_time(current_time_seconds) {}
	}
}
