#include "pch.h"
#include "wave_state.h"

#include "commands/remove_entropy_command.h"

wave_state::wave_state(int number_to_reserve) : current_index(-1), number_of_failures(0)
{
	commands.reserve(number_to_reserve);
}

void wave_state::execute_command(remove_entropy_command command)
{
	// Removes old states
	for (int i = commands.size() - 1; i > current_index; --i)
	{
		commands.pop_back();
	}

	commands.emplace_back(std::move(command));
	commands[++current_index].execute();
}

void wave_state::clear()
{
	while(!empty())
	{
		undo();
	}
}

void wave_state::rewind(SDL_Point broken_position)
{
	if (last_broken_position == broken_position)
	{
		number_of_failures++;
	}

	for (auto i = 0; i < (number_of_failures + 1) * 2; ++i)
	{
		undo();
	}

	last_broken_position = broken_position;
}


void wave_state::undo()
{
	if (current_index < 0)
	{
		return;
	}

	commands[current_index--].undo();
}

bool wave_state::empty() const
{
	return current_index < 0;
}

size_t wave_state::size() const
{
	return commands.size();
}
