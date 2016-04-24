#ifndef DUNJUN_GAME_HPP
#define DUNJUN_GAME_HPP

#include <Dunjun/Level.hpp>

namespace Dunjun
{
	namespace Game
	{
		void init();
		void run();
		void cleanUp();

		GLFWwindow* getGlfwWindow();
		Vector2 getWindowSize();


	} // end Game
} // end Dunjun

#endif
