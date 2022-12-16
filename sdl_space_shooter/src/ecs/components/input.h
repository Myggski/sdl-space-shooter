#pragma once

#include <SDL_scancode.h>
#include "component.h"

namespace ecs
{
	namespace components
	{
		struct input : component<input, component_types::input>
		{
		public:
			input();
			input(SDL_Scancode move_up, SDL_Scancode move_down, SDL_Scancode move_left, SDL_Scancode move_right, SDL_Scancode fire_laser);

			float x;
			float y;
			bool is_firing;
			SDL_Scancode move_up;
			SDL_Scancode move_down;
			SDL_Scancode move_left;
			SDL_Scancode move_right;
			SDL_Scancode fire_laser;
		};

	}
}
