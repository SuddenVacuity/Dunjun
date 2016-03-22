#ifndef DUNJUN_CLOCK_HPP
#define DUNJUN_CLOCK_HPP

#include <Dunjun/OpenGL.hpp>

#include <GLFW/glfw3.h>

namespace Dunjun
{
	class Clock
	{
	public:
		inline f64 getElapsedTime() const
		{
			return glfwGetTime() - m_startTime;
		}

		inline f64 restart()
		{
			f64 now = glfwGetTime();
			f64 elapsed = now - m_startTime;
			m_startTime = now;

			return elapsed;
		}

	private:
		f64 m_startTime = glfwGetTime();
	};

}

#endif
