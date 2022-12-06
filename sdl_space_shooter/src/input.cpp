#include "pch.h"
#include "input.h"

input::input() : registered_events({}) { }

void input::add_listener(const SDL_Scancode& scancode, const input_callback* callback) {
	if (registered_events.count(scancode))
	{
		registered_events.at(scancode).push_back(callback);
	}
	else
	{
		registered_events.insert(std::make_pair(scancode, std::vector<const input_callback*>(1, { callback })));
	}
}

void input::remove_listener(const SDL_Scancode& scancode, const input_callback* callback) {
	std::vector<const input_callback*>& callbacks = registered_events.at(scancode);

	auto is_input_callback_same = [&](const input_callback* event_data) {
		return (*event_data).target<input_callback>() == (*callback).target<input_callback>();
	};

	callbacks.erase(
		std::remove_if(callbacks.begin(), callbacks.end(), is_input_callback_same), callbacks.end());

	if (callbacks.empty())
	{
		registered_events.erase(registered_events.find(scancode));
	}
}

void input::clear()
{
	while (!registered_events.empty())
	{
		const auto& registered_event = *registered_events.begin();
		for (const auto& callback : registered_event.second)
		{
			remove_listener(registered_event.first, callback);
		}
	}

	registered_events.clear();
}