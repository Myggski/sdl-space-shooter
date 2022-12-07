#pragma once

#include <SDL_scancode.h>
#include "components.h"
#include "ecs/component.h"

namespace components
{
	struct input : ecs::component<input, component_type::input>
	{
	public:
		input();
		input(SDL_Scancode move_up, SDL_Scancode move_down, SDL_Scancode move_left, SDL_Scancode move_right);

		SDL_Scancode move_up;
		SDL_Scancode move_down;
		SDL_Scancode move_left;
		SDL_Scancode move_right;
	};
}