#include "pch.h"

#include "ecs/components/input.h"

namespace ecs
{
	namespace components
	{
		input::input()
			: x(0.f), y(0.f), is_firing(false), move_up(SDL_SCANCODE_UNKNOWN), move_down(SDL_SCANCODE_UNKNOWN), move_left(SDL_SCANCODE_UNKNOWN), move_right(SDL_SCANCODE_UNKNOWN), fire_laser(SDL_SCANCODE_UNKNOWN) { }
		input::input(SDL_Scancode move_up, SDL_Scancode move_down, SDL_Scancode move_left, SDL_Scancode move_right, SDL_Scancode fire_laser)
			: x(0.f), y(0.f), is_firing(false), move_up(move_up), move_down(move_down), move_left(move_left), move_right(move_right), fire_laser(fire_laser) { }
	}
}
