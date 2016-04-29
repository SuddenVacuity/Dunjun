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
			// preset window sizes
			const int windowSize_1366_768[2] = {1366, 768};
			const int windowSize_1280_600[2] = { 1280, 600 };
			const int windowSize_854_480[2] = { 854, 480 };

			// default window variables
			int width = windowSize_854_480[0];
			int height = windowSize_854_480[1];
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
