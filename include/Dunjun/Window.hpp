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
			const int windowSize_854_480[2] = { 854, 480 };
			const int windowSize_1366_768[2] = {1366, 768};
			const int windowSize_1280_600[2] = { 1280, 600 };

			// preset aspect ratios
			f32 windowAspect_16_9 = 16.0f / 9.0f;

			// window variables
			int width = windowSize_854_480[0];
			int height = windowSize_854_480[1];
			f32 aspectRatio = windowAspect_16_9;
			bool isFullscreen = false;

		} // end anon namespace

		extern GLFWwindow* ptr;

		GLFWwindow* getHandle();
		void setHandle(GLFWwindow* w);

		//extern bool getIsFullscreen();
		//void setFullscreen(bool fullscreen, GLFWmonitor* monitor);

		bool init();
		void cleanup();

		void destroyWindow();
		void destroyWindow(GLFWwindow* w);

		GLFWwindow* createWindow(GLFWmonitor* monitor);
		GLFWwindow* createWindow(GLFWmonitor* monitor, u32 width, u32 height);

		void makeContextCurrent();
		void swapInterval(int i);

		void swapBuffers();
		void pollEvents();

		void setTitle(const std::string& title);

		//bool isFullscreen();
		//void setFullscreen(bool fullscreen);

		Vector2 getWindowSize();
		Vector2 getFramebufferSize();

		bool shouldClose();
		bool isInFocus();
		bool isIconified();

		//void setIcon(const Image& image);
		//void setFramerateLimit(u32 limit);
		//void setVerticalSyncEnabled(bool enabled);
		//bool hasFocus();
		//
		//bool pollEvent(Event& event);
		//bool waitEvent(Event& event);

	} // end Window
} // end Dunjun

#endif
