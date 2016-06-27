#ifndef DUNJUN_SYSTEM_TICKCOUNTER_HPP
#define DUNJUN_SYSTEM_TICKCOUNTER_HPP

#include <Dunjun/System/Clock.hpp>

namespace Dunjun
{
	struct TickCounter
	{
	public:
		std::size_t tick = 0;
		f64 tickRate = 0;
		Clock clock = Clock();

		bool update(Time frequency) // frequency is the time between tick rate updates
		{
			bool reset = false;
			if (clock.getElapsedTime() >= frequency)
			{
				tickRate = tick * (1.0f / frequency.asSeconds());
				tick = 0;
				reset = true;
				clock.restart();
			}
			tick++;
			return reset;
		}
	};
}

#endif
