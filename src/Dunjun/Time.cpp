
#include <Dunjun/Time.hpp>

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

	Time Time::now()
	{
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();

		return microseconds(std::chrono::duration_cast
			<std::chrono::microseconds>(now).count());
	}

	void Time::sleep(Time time)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(time.asMicroseconds()));
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
