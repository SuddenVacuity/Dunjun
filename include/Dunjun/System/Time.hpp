#ifndef DUNJUN_SYSTEM_TIME_HPP
#define DUNJUN_SYSTEM_TIME_HPP

#include <Dunjun/Window/ContextSettings.hpp>

//#include <chrono> // included in Common.hpp

namespace Dunjun
{
	class Time
	{
	public:
		Time() = default;

		GLOBAL const Time Zero;

		f32 asSeconds() const;
		s32 asMilliseconds() const;
		s64 asMicroseconds() const;

		GLOBAL Time now();
		GLOBAL void sleep(Time time);

	private:
		friend Time seconds(f32);
		friend Time milliseconds(s32);
		friend Time microseconds(s64);

		explicit Time(s64 microseconds);

		std::chrono::microseconds m_microseconds = std::chrono::microseconds(0);
	}; // end Time

	//const Time Zero = seconds(0);

	Time seconds(f32 amount);
	Time milliseconds(s32 amount);
	Time microseconds(s64 amount);

	//
	//OPERATORS
	bool operator==(Time left, Time right);
	bool operator!=(Time left, Time right);

	bool operator>(Time left, Time right);
	bool operator<(Time left, Time right);
	bool operator>=(Time left, Time right);
	bool operator<=(Time left, Time right);

	inline Time operator-(Time right);

	//ADDITION AND SUBTRACTION
	Time operator+(Time left, Time right);
	Time operator-(Time left, Time right);
	Time& operator+=(Time& left, Time right);
	Time& operator-=(Time& left, Time right);

	//MULTIPLICATION
	Time operator*(Time left, f32 right);
	Time operator*(f32 left, Time right);
	Time operator*(Time left, s64 right);
	Time operator*(s64 left, Time right);

	Time& operator*=(Time& left, f32 right);
	Time& operator*=(Time& left, s64 right);
			
	//DIVISION
	Time operator/(Time left, f32 right);
	Time operator/(Time left, s64 right);
	Time& operator/=(Time& left, f32 right);
	Time& operator/=(Time& left, s64 right);
	f32 operator/(Time left, Time right);

	Time operator%(Time left, Time right);
	Time& operator%=(Time& left, Time right);

} // end Dunjun

#endif
