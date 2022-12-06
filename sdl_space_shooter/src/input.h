#pragma once

#include "application/events.h"

using input_callback = std::function<void(const SDL_EventType&)>;
using key_event_map = std::unordered_map<SDL_Scancode, std::vector<const input_callback*>>;

class input final
{
private:
	key_event_map registered_events;
public:
	input();
	void add_listener(const SDL_Scancode& scancode, const input_callback* callback);
	void remove_listener(const SDL_Scancode& scancode, const input_callback* callback);
	void clear();
	application::event_callback invoke_key_press = [this](SDL_Event event)
	{
		if (registered_events.empty())
		{
			return;
		}

		const SDL_Scancode scancode = event.key.keysym.scancode;
		const SDL_EventType event_type = static_cast<SDL_EventType>(event.type);

		if (registered_events.count(scancode))
		{
			const std::vector<const input_callback*>& callbacks = registered_events.at(scancode);
			for (const input_callback* callback : callbacks)
			{
				(*callback)(event_type);
			}
		}
	};
};