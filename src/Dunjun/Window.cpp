
#include <Dunjun/Window.hpp>

namespace Dunjun
{
	namespace Window
	{
		GLFWwindow* ptr;


		// prototypes for callback functions
		INTERNAL void resizeCallback(GLFWwindow* window, int width, int height);
		INTERNAL void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		INTERNAL void errorCallback(int error, const char* description);
		INTERNAL void windowRefreshCallback(GLFWwindow* window);

		bool init()
		{
			if(!glfwInit())
				return false;

			Window::ptr = createWindow(nullptr);

			if(!Window::ptr)
			{
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(Window::ptr); // set the context for the window
			glfwSwapInterval(1);

			glfwSetErrorCallback(errorCallback);


			return true;
		}

		void cleanup()
		{
			glfwDestroyWindow(Window::ptr);
			glfwTerminate();
		}

		void destroyWindow()
		{
			glfwDestroyWindow(Window::ptr);
		}

		void destroyWindow(GLFWwindow* windowPtr)
		{
			glfwDestroyWindow(windowPtr);
		}

		GLFWwindow* createWindow(GLFWmonitor* monitor)
		{
			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // glfw version ex) 2.1
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // glfw version ex) 2.1
			glfwWindowHint(GLFW_FOCUSED, true); // when window is created focus on it

			if (monitor) // fullscreen
			{
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);	// set variable to current monitor resolution

				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);	// set window context to match monitor
				glfwWindowHint(GLFW_RED_BITS, mode->redBits);			// set window context to match monitor
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);		// set window context to match monitor
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);			// set window context to match monitor

				glfwWindowHint(GLFW_RESIZABLE, false);					// prevent fullscreen window form being resizable

				Window::width = mode->width;							// set window resolution to match monitor
				Window::height = mode->height;							// set window resolution to match monitor
			}
			else // not full screen
			{
				glfwWindowHint(GLFW_RESIZABLE, true);

				// default size
				Window::width = windowSize_854_480[0];
				Window::height = windowSize_854_480[1];
				Window::isFullscreen = false;
			}

			GLFWwindow* w = glfwCreateWindow(Window::width, Window::height, "Title: Dunjun!", monitor, Window::ptr);

			// set GLFW specific Callbacks
			glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
			glfwSetWindowSizeCallback(w, resizeCallback);
			glfwSetWindowRefreshCallback(w, windowRefreshCallback);

			glfwGetWindowSize(w, &Window::width, &Window::height);

			return w;
		}

		GLFWwindow* createWindow(GLFWmonitor* monitor, u32 width, u32 height)
		{
			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // glfw version ex) 2.1
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // glfw version ex) 2.1
			glfwWindowHint(GLFW_FOCUSED, true); // when window is created focus on it

			if (monitor) // fullscreen
			{
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);	// set variable to current monitor resolution

				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);	// set window context to match monitor
				glfwWindowHint(GLFW_RED_BITS, mode->redBits);			// set window context to match monitor
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);		// set window context to match monitor
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);			// set window context to match monitor

				glfwWindowHint(GLFW_RESIZABLE, false);					// prevent fullscreen window form being resizable

				Window::width = mode->width;							// set window resolution to match monitor
				Window::height = mode->height;							// set window resolution to match monitor
			}
			else // not full screen
			{
				glfwWindowHint(GLFW_RESIZABLE, true);

				// default size
				Window::width = width;
				Window::height = height;
				Window::isFullscreen = false;
			}

			GLFWwindow* w = glfwCreateWindow(Window::width, Window::height, "Title: Dunjun!", monitor, Window::ptr);

			// set GLFW specific Callbacks
			glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
			glfwSetWindowSizeCallback(w, resizeCallback);
			glfwSetWindowRefreshCallback(w, windowRefreshCallback);

			glfwGetWindowSize(w, &Window::width, &Window::height);

			return w;
		}

		void makeContextCurrent()
		{
			glfwMakeContextCurrent(Window::ptr);
		}

		void swapInterval(int i)
		{
			glfwSwapInterval(i);
		}

		void swapBuffers()
		{
			glfwSwapBuffers(Window::ptr);
		}

		void pollEvents()
		{
			glfwPollEvents();
		}

		//bool getIsFullscreen()
		//{
		//	return isFullscreen;
		//}
		//
		//void setFullscreen(bool fullscreen, GLFWmonitor* monitor)
		//{
		//	if(fullscreen == getIsFullscreen())
		//		return;
		//
		//
		//}

		void setTitle(const char* title)
		{
			glfwSetWindowTitle(Window::ptr, title);
		}

		Vector2 getWindowSize()
		{
			return Vector2(static_cast<f32>(Window::width), static_cast<f32>(Window::height));
		}

		Vector2 getFramebufferSize()
		{
			int width;
			int height;
			
			glfwGetFramebufferSize(Window::ptr, &width, &height);
			
			return Vector2(width, height);
		}

		bool shouldClose()
		{
			return glfwWindowShouldClose(Window::ptr) == 1;
		}

		bool isInFocus()
		{
			return glfwGetWindowAttrib(Window::ptr, GLFW_FOCUSED) == 1;
		}

		bool isIconified()
		{
			return glfwGetWindowAttrib(Window::ptr, GLFW_ICONIFIED) == 1;
		}

		// callback functions
		INTERNAL void resizeCallback(GLFWwindow* window, int width, int height)
		{
			Window::width = width;
			Window::height = height;
		}

		INTERNAL void framebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		INTERNAL void errorCallback(int error, const char* description)
		{
			std::cerr << "[ERROR]";
			std::cerr << "GLFW: " << description;
			std::cerr << std::endl;
		}

		INTERNAL void windowRefreshCallback(GLFWwindow* window)
		{
			Vector2 fbSize = getFramebufferSize();
			glViewport(0, 0, (GLsizei)fbSize.x, (GLsizei)fbSize.y);

			glfwMakeContextCurrent(window);
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSwapBuffers(window);
		}

	} // end Window
} // end Dunjun