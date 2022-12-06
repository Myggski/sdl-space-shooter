#pragma once

namespace application
{
	struct timer final {
	public:
		timer();
		void init();
		void refresh_dt();
		const float& delta_time;
	private:
		Uint64 previous_ticks;
		float dt;
	};
}