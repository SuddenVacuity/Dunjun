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

		inline Time getElapsedTime() const
		{
			return Time::now() - startTime;
		}

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
