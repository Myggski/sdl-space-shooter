#include "pch.h"
#include "input.h"

namespace ecs
{
	namespace components
	{
		input::input()
			: move_up(SDL_SCANCODE_UNKNOWN), move_down(SDL_SCANCODE_UNKNOWN), move_left(SDL_SCANCODE_UNKNOWN), move_right(SDL_SCANCODE_UNKNOWN) { }
		input::input(SDL_Scancode move_up, SDL_Scancode move_down, SDL_Scancode move_left, SDL_Scancode move_right)
			: move_up(move_up), move_down(move_down), move_left(move_left), move_right(move_right) { }
	}
}