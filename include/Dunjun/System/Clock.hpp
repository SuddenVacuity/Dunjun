#ifndef DUNJUN_SYSTEM_CLOCK_HPP
#define DUNJUN_SYSTEM_CLOCK_HPP

//#include <Dunjun/Input.hpp>
#include <Dunjun/System/Time.hpp>

//#include <thread> // included in Common.hpp

namespace Dunjun
{
	struct Clock
	{
	public:
		Time startTime = Time::now();

		// returns time since the clock was started or last restarted
		inline Time getElapsedTime() const
		{
			return Time::now() - startTime;
		}

		// restarts the clock
		// set start time to current time
		// returns elapsed time since previous start time
		inline Time restart()
		{
			//Time now = Input::getTime();
			Time now = Time::now();
			Time elapsed = now - startTime;
			startTime = now;

			return elapsed;
		}
	};
}

#endif
