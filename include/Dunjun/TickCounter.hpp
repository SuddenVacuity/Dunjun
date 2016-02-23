#ifndef DUNJUN_TICKCOUNTER_HPP
#define DUNJUN_TICKCOUNTER_HPP

#include <Dunjun/CLock.hpp>

namespace Dunjun
{
	class TickCounter
	{
	public:
		bool update(double frequency) // frequency is the time between tick rate updates
		{
			bool reset = false;
			if (m_clock.getElapsedTime() >= frequency)
			{
				m_tickRate = m_tick / frequency;
				m_tick = 0;
				reset = true;
				m_clock.restart();
			}
			m_tick++;
			return reset;
		}

		inline std::size_t getTickRate() const { return m_tickRate; }

	private:
		std::size_t m_tick = 0;
		std::size_t m_tickRate = 0;
		Clock m_clock;

	};
}

#endif