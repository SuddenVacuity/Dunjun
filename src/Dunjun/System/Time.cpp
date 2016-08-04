
#include <Dunjun/System/Time.hpp>

//#include <thread> // included in Common.hpp

#if defined(DUNJUN_COMPILER_MSVC)
	#include <Windows.h>
#endif

namespace Dunjun
{
	const Time Time::Zero = Time(0);

	f32 Time::asSeconds() const
	{
		return m_microseconds.count() / 1000000.0f;
	}

	s32 Time::asMilliseconds() const
	{
		return static_cast<s32>(m_microseconds.count() / 1000);
	}

	s64 Time::asMicroseconds() const
	{
		return m_microseconds.count();
	}

#if defined(DUNJUN_COMPILER_MSVC)
	namespace
	{
		INTERNAL LARGE_INTEGER getFrequency()
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency;
		}
	} // end anon namespace

	Time Time::now()
	{
		// force to run on first core
		// See http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
		HANDLE currentThread = GetCurrentThread();
		DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

		// Get the frequency of the performance counter
		// It is constant across the program's lifetime
		LOCAL_PERSIST LARGE_INTEGER s_frequency = getFrequency();

		// current time
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		// restore thread affinity
		SetThreadAffinityMask(currentThread, previousMask);

		return microseconds(1000000 * time.QuadPart / s_frequency.QuadPart);

	}
#else

	Time Time::now()
	{
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();

		return microseconds(std::chrono::duration_cast<std::chrono::microseconds>(now).count());
	}

#endif

	void Time::sleep(Time time)
	{
		if(time.asMicroseconds() <= 0)
			return;
#if defined(DUNJUN_COMPILER_MSVC)
		// get supported time resolutions
		TIMECAPS tc;
		timeGetDevCaps(&tc, sizeof(TIMECAPS));

		// set the time resolution to the minimum for the sleep call
		timeBeginPeriod(tc.wPeriodMin);

		// wait
		::Sleep(time.asMilliseconds());

		// reset the timer resolution back to the system default
		timeBeginPeriod(tc.wPeriodMin);
#else
		std::this_thread::sleep_for(std::chrono::microseconds(time.asMicroseconds()));
#endif
	}

	//
	// PRIVATE
	Time::Time(s64 microseconds)
		: m_microseconds(std::chrono::microseconds(microseconds))
	{
	}

Time seconds(f32 amount)
{
	return Time(static_cast<s64>(amount * 1000000));
}

Time milliseconds(s32 amount)
{
	return Time(static_cast<s64>(amount * 1000));
}

Time microseconds(s64 amount)
{
	return Time(amount);
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


bool operator==(Time left, Time right)
{
	return left.asMicroseconds() == right.asMicroseconds();
}

bool operator!=(Time left, Time right)
{
	return left.asMicroseconds() != right.asMicroseconds();
}

bool operator>(Time left, Time right)
{
	return left.asMicroseconds() > right.asMicroseconds();
}

bool operator<(Time left, Time right)
{
	return left.asMicroseconds() < right.asMicroseconds();
}

bool operator>=(Time left, Time right)
{
	return left.asMicroseconds() >= right.asMicroseconds();
}

bool operator<=(Time left, Time right)
{
	return left.asMicroseconds() <= right.asMicroseconds();
}

Time operator-(Time right)
{
	return microseconds(-right.asMicroseconds());
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					ADDITION AND SUBTRACTION
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

Time operator+(Time left, Time right)
{
	return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time operator-(Time left, Time right)
{
	return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time& operator+=(Time& left, Time right)
{
	return left = left + right;
}

Time& operator-=(Time& left, Time right)
{
	return left = left - right;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					MULTIPLICATION
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

Time operator*(Time left, f32 right)
{
	return microseconds(left.asSeconds() * right);
}

Time operator*(f32 left, Time right)
{
	return microseconds(left * right.asSeconds());
}

Time operator*(Time left, s64 right)
{
	return microseconds(left.asMicroseconds() * right);
}
Time operator*(s64 left, Time right)
{
	return microseconds(left * right.asMicroseconds());
}

Time& operator*=(Time& left, f32 right)
{
	return left = left * right;
}

Time& operator*=(Time& left, s64 right)
{
	return left = left * right;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					DIVISION
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

Time operator/(Time left, f32 right)
{
	return microseconds(left.asSeconds() / right);
}

Time operator/(Time left, s64 right)
{
	return microseconds(left.asMicroseconds() / right);
}

Time& operator/=(Time& left, f32 right)
{
	return left = left / right;
}

Time& operator/=(Time& left, s64 right)
{
	return left = left / right;
}

f32 operator/(Time left, Time right)
{
	return left.asSeconds() / right.asSeconds();
}

Time operator%(Time left, Time right)
{
	return microseconds(left.asMicroseconds() % right.asMicroseconds());
}

Time& operator%=(Time& left, Time right)
{
	return left = left % right;
}

} // end Dunjun
