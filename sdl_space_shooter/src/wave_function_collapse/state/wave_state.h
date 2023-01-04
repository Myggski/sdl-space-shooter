#pragma once

#include "commands/remove_entropy_command.h"

class wave_state
{
public:
	wave_state(int number_to_reserve);
	void execute_command(remove_entropy_command command);
	void rewind(SDL_Point broken_position);
	void clear();
	bool empty() const;
	size_t size() const;

private:
	void undo();

private:
	std::vector<remove_entropy_command> commands;
	int current_index;
	int number_of_failures;
	SDL_Point last_broken_position;
};

