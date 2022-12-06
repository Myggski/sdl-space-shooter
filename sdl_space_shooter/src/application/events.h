#pragma once

#include <functional>
#include <unordered_map>
#include <SDL_events.h>

namespace application
{
	using event_callback = std::function<void(const SDL_Event&)>;
	using event_map = std::unordered_map<SDL_EventType, std::vector<event_callback*>>;

	class events final
	{
		;
	public:
		events();
		void pull() const;
		void add_listener(const SDL_EventType& eventType, event_callback* callback);
		void remove_listener(const SDL_EventType& eventType, event_callback* callback);
		void clear();
		inline void setup_input_events(event_callback* key_press_callback)
		{
			add_listener(SDL_KEYDOWN, key_press_callback);
			add_listener(SDL_KEYUP, key_press_callback);
		}
	private:
		event_map registered_events;
		void trigger_event(const SDL_Event& event) const;
	};
}