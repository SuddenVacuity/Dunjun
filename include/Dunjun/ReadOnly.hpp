/*#ifndef DUNJUN_READONLY_HPP
#define DUNJUN_READONLY_HPP 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
)				REMOVED FROM PROJECT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include <Dunjun/ResourceHolder.hpp>

namespace Dunjun   
{
	// outside functions can only access const references from this class
	// this class can access all varibles from classes in Super
	template <class T, class Super>
	class ReadOnly
	{
	public:
		operator const T&() const { return data; }

		const T* operator&() const
		{
			return &data;
		}
		
	private:
		friend Super;


		ReadOnly()
			: data()
		{
		}

		ReadOnly(const T& t)
			: data(t)
		{
		}

		ReadOnly(T&& t)
			: data(std::move(t))
		{
		}

		ReadOnly& operator=(const T& t)
		{
			data = t;
			return *this;
		}

		T* operator&()
		{
			return &data;
		}

		template <class U>
		U& operator*()
		{
			return *data;
		}

		bool operator==(const T& t) const
		{
			return data == t;
		}
		bool operator!=(const T& t) const
		{
			return !operator==(t);
		}

		// Does not use 'm_' prefix as its accessable within the Super class so
		// it is not technically private for that class (friend)
		// This will be the only exception to the private member variable prefix
		// rule of 'm_'
		T data;

	};
} // end Dunjun   
*/
/*EXAMPLE
=========================
#include <cstdio>

struct Thing
{
	Thing()
		: x()
	{
	}

	Thing(int x)
		: x(x)
	{
	}

	void increment()
	{
		x.data++; // get raw data from x and increment
	}

	ReadOnly<int, Thing> x; // what would normally be private
	};

void func()
{
	Thing a(2);				  // initialize a.x = 2;
	printf(i%\n", (int)a.x);  // prints 2
							  // a.x must be cast as in int since 
							  // it's type is ReadOnly<int, Thing>
	// a.x = 7 // error
	a.increment();
	printf(%i\n, (int)a.x);   // prints 3
}


*/// end EXAMPLE
//#endif

