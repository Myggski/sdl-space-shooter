#include "pch.h"

#include "damage.h"

namespace ecs
{
	namespace components
	{
		damage::damage()
			: damage_to_deal(0) {}
		damage::damage(int damage_to_deal)
			: damage_to_deal(damage_to_deal) {}
	}
}
