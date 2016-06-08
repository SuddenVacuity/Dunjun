#ifndef DUNJUN_CLOCK_HPP
#define DUNJUN_CLOCK_HPP

#include <Dunjun/Input.hpp>

//#include <thread> // included in Common.hpp

namespace Dunjun
{
	class Clock
	{
	public:
		inline Time getElapsedTime() const
		{
			return Time::now() - m_startTime;
		}

		inline Time restart()
		{
			//Time now = Input::getTime();
			Time now = Time::now();
			Time elapsed = now - m_startTime;
			m_startTime = now;

			return elapsed;
		}

	private:
		Time m_startTime = Time::now();
	};
}

#endif
