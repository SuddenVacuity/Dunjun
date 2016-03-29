#ifndef DUNJUN_GAME_HPP
#define DUNJUN_GAME_HPP

#include <Dunjun/ShaderProgram.hpp>
#include <vector>

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
