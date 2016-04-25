#ifndef  DUNJUN_WINDOW_HPP
#define  DUNJUN_WINDOW_HPP

#include <Dunjun/OpenGL.hpp>

#include <GLFW/glfw3.h>

//struct GLFWwindow;
//struct GLFWmonitor;

namespace Dunjun
{
	namespace Window
	{
		namespace
		{
			int width = 854;
			int height = 480;
			bool isFullscreen = false;
		} // end anon namespace

		extern GLFWwindow* ptr;

		//extern bool getIsFullscreen();
		//void setFullscreen(bool fullscreen, GLFWmonitor* monitor);

		bool init();
		void cleanup();

		void destroyWindow();
		void destroyWindow(GLFWwindow* windowPtr);

		GLFWwindow* createWindow(GLFWmonitor* monitor);

		void makeContextCurrent();
		void swapInterval(int i);

		void swapBuffers();
		void pollEvents();

		void setTitle(const char* title);

		Vector2 getWindowSize();
		Vector2 getFramebufferSize();

		bool isInFocus();
		bool isIconified();

	} // end Window
} // end Dunjun

#endif
