
#include <Dunjun/Window.hpp>

namespace Dunjun
{
	namespace
	{
		const Window* fullscreenWindow = nullptr;

		INTERNAL u32 generateFlags(u32 style)
		{
			u32 flags = 0;

			flags |= SDL_WINDOW_OPENGL;

			// these are in SDL order
			if (style & Style::Fullscreen && !(Style::Windowed))
				flags |= SDL_WINDOW_FULLSCREEN;
			if (style & Style::Visible)
				flags |= SDL_WINDOW_SHOWN;
			if (style & Style::Hidden)
				flags |= SDL_WINDOW_HIDDEN;
			if (style & Style::Borderless)
				flags |= SDL_WINDOW_BORDERLESS;
			if (style & Style::Resizable)
				flags |= SDL_WINDOW_RESIZABLE;
			if (style & Style::Minimized)
				flags |= SDL_WINDOW_MINIMIZED;
			if (style & Style::Maximized)
				flags |= SDL_WINDOW_MAXIMIZED;
			if (style & Style::Fullscreen && Style::Windowed)
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

			return flags;
		}
	} // end anon namespace



	Window::Window()
		: m_impl(nullptr)
		, m_context()
		, m_frameTimeLimit(Time::Zero)
		, m_size(0, 0)
	{
	}

	Window::Window(const std::string& title,
				   const u32& width, const u32& height,
				   u32 style)
		: m_impl(nullptr)
		, m_context()
		, m_frameTimeLimit(Time::Zero)
		, m_size(0, 0)
	{
		create(title, width, height, style);
	}

	Window::~Window()
	{
		// possibly need to make it this->close();
		close();
	}

	void Window::create(const std::string& title,
						const u32& width, const u32& height,
						u32 style)
	{
		// destroy the original window
		// possibly need to make it this->close();
		close();

		if(style & Style::Fullscreen)
		{
			if(fullscreenWindow)
			{
				std::cerr << "Creating two fullscreen windows is not allowed!" << std::endl;
				style &= ~Style::Fullscreen;
			}
			else
			{
				// check fullscreen dimenstions are valid

				fullscreenWindow = this;
			}
		}

		u32 windowFlags = generateFlags(style);

		m_impl = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								  width, height, windowFlags);

		m_context = SDL_GL_CreateContext(m_impl);

		m_size = Var2_int(width, height);

		init();
	}

	void Window::init()
	{
		setVisible(true);
		setFramerateLimit(0);
		SDL_GetWindowSize(m_impl, &m_size.x, &m_size.y);
	}

	void Window::close()
	{
		SDL_DestroyWindow(m_impl);
		m_impl = nullptr;

		if(this == fullscreenWindow)
			fullscreenWindow = nullptr;
	}

	bool Window::isOpen() const
	{
		return m_impl != nullptr;
	}

	Vector2 Window::getPosition() const
	{
		if(m_impl)
		{
			int x, y;
			SDL_GetWindowPosition(m_impl, &x, &y);

			return Vector2(x, y);
		}
		
		return {0, 0};
	}

	Window& Window::setPosition(const Vector2& position)
	{
		SDL_SetWindowPosition(m_impl, position.x, position.y);

		return *this;
	}

	Var2_int Window::getSize() const
	{
		return m_size;
	}

	Window& Window::setSize(const u32 width, const u32 height)
	{
		SDL_SetWindowSize(m_impl, width, height);
		SDL_GetWindowSize(m_impl, &m_size.x, &m_size.y);

		return *this;
	}

	Window& Window::setTitle(std::string& title)
	{
		SDL_SetWindowTitle(m_impl, title.c_str());

		return *this;
	}

	Window& Window::setVisible(bool visible)
	{
		return *this;
	}

	Window& Window::setVerticalSyncEnabled(bool enabled)
	{
		return *this;
	}

	Window& Window::setFramerateLimit(u32 limit)
	{
		if (limit > 0)
			m_frameTimeLimit = seconds(1.0f / (f32)limit);
		else
			m_frameTimeLimit = Time::Zero;

		return *this;
	}

	void Window::display()
	{
		SDL_GL_SwapWindow(m_impl);

		if(m_frameTimeLimit != Time::Zero)
		{
			Time::sleep(m_frameTimeLimit - m_clock.getElapsedTime());
			m_clock.restart();
		}
	}








	//namespace Window
	//{
	//	GLFWwindow* ptr;
	//
	//
	//	// prototypes for callback functions
	//	INTERNAL void resizeCallback(GLFWwindow* window, int width, int height);
	//	INTERNAL void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	//	INTERNAL void errorCallback(int error, const char* description);
	//	INTERNAL void windowRefreshCallback(GLFWwindow* window);
	//
	//	GLFWwindow* getHandle()
	//	{
	//		return ptr;
	//	}
	//
	//	void setHandle(GLFWwindow* w)
	//	{
	//		ptr = w;
	//	}
	//
	//	bool init()
	//	{
	//		if(!glfwInit())
	//			return false;
	//
	//		setHandle(createWindow(nullptr, Window::width, Window::height));
	//
	//		if(!getHandle())
	//		{
	//			glfwTerminate();
	//			return false;
	//		}
	//
	//		//glfwMakeContextCurrent(getHandle()); // set the context for the window
	//		//glfwSwapInterval(1);
	//		makeContextCurrent();
	//
	//		glfwSetErrorCallback(errorCallback);
	//
	//		return true;
	//	}
	//
	//	void cleanup()
	//	{
	//		glfwDestroyWindow(getHandle());
	//		glfwTerminate();
	//	}
	//
	//	void destroyWindow()
	//	{
	//		glfwDestroyWindow(getHandle());
	//	}
	//
	//	void destroyWindow(GLFWwindow* windowPtr)
	//	{
	//		glfwDestroyWindow(windowPtr);
	//	}
	//
	//	GLFWwindow* createWindow(GLFWmonitor* monitor)
	//	{
	//		glfwDefaultWindowHints();
	//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // glfw version ex) 2.1
	//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // glfw version ex) 2.1
	//		glfwWindowHint(GLFW_FOCUSED, true); // when window is created focus on it
	//
	//		if (monitor) // fullscreen
	//		{
	//			const GLFWvidmode* mode = glfwGetVideoMode(monitor);	// set variable to current monitor resolution
	//
	//			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);	// set window context to match monitor
	//			glfwWindowHint(GLFW_RED_BITS, mode->redBits);			// set window context to match monitor
	//			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);		// set window context to match monitor
	//			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);			// set window context to match monitor
	//
	//			glfwWindowHint(GLFW_RESIZABLE, false);					// prevent fullscreen window form being resizable
	//
	//			Window::width = mode->width;							// set window resolution to match monitor
	//			Window::height = mode->height;							// set window resolution to match monitor
	//		}
	//		else // not full screen
	//		{
	//			glfwWindowHint(GLFW_RESIZABLE, true);
	//
	//			// default size
	//			Window::width = windowSize_854_480[0];
	//			Window::height = windowSize_854_480[1];
	//			Window::isFullscreen = false;
	//		}
	//
	//		GLFWwindow* w = glfwCreateWindow(Window::width, Window::height, "Title: Dunjun!", monitor, getHandle());
	//
	//		// set GLFW specific Callbacks
	//		glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
	//		glfwSetWindowSizeCallback(w, resizeCallback);
	//		glfwSetWindowRefreshCallback(w, windowRefreshCallback);
	//
	//		glfwGetWindowSize(w, &Window::width, &Window::height);
	//
	//		return w;
	//	}
	//
	//	GLFWwindow* createWindow(GLFWmonitor* monitor, u32 width, u32 height)
	//	{
	//		glfwDefaultWindowHints();
	//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // glfw version ex) 2.1
	//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // glfw version ex) 2.1
	//		glfwWindowHint(GLFW_FOCUSED, true); // when window is created focus on it
	//
	//		if (monitor) // fullscreen
	//		{
	//			const GLFWvidmode* mode = glfwGetVideoMode(monitor);	// set variable to current monitor resolution
	//
	//			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);	// set window context to match monitor
	//			glfwWindowHint(GLFW_RED_BITS, mode->redBits);			// set window context to match monitor
	//			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);		// set window context to match monitor
	//			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);			// set window context to match monitor
	//
	//			glfwWindowHint(GLFW_RESIZABLE, false);					// prevent fullscreen window form being resizable
	//
	//			Window::width = mode->width;							// set window resolution to match monitor
	//			Window::height = mode->height;							// set window resolution to match monitor
	//		}
	//		else // not full screen
	//		{
	//			glfwWindowHint(GLFW_RESIZABLE, true);
	//
	//			// default size
	//			Window::width = width;
	//			Window::height = height;
	//			Window::isFullscreen = false;
	//		}
	//
	//		GLFWwindow* w = glfwCreateWindow(Window::width, Window::height, "Title: Dunjun!", monitor, getHandle());
	//
	//		// set GLFW specific Callbacks
	//		glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
	//		glfwSetWindowSizeCallback(w, resizeCallback);
	//		glfwSetWindowRefreshCallback(w, windowRefreshCallback);
	//
	//		glfwGetWindowSize(w, &Window::width, &Window::height);
	//
	//		return w;
	//	}
	//
	//	void makeContextCurrent()
	//	{
	//		glfwMakeContextCurrent(getHandle());
	//	}
	//
	//	void swapInterval(int i)
	//	{
	//		glfwSwapInterval(i);
	//	}
	//
	//	void swapBuffers()
	//	{
	//		glfwSwapBuffers(getHandle());
	//	}
	//
	//	void pollEvents()
	//	{
	//		glfwPollEvents();
	//	}
	//
	//	//bool getIsFullscreen()
	//	//{
	//	//	return isFullscreen;
	//	//}
	//	//
	//	//void setFullscreen(bool fullscreen, GLFWmonitor* monitor)
	//	//{
	//	//	if(fullscreen == getIsFullscreen())
	//	//		return;
	//	//
	//	//
	//	//}
	//
	//	void setTitle(const std::string& title)
	//	{
	//		glfwSetWindowTitle(getHandle(), title.c_str());
	//	}
	//
	//	Vector2 getWindowSize()
	//	{
	//		return Vector2(static_cast<f32>(Window::width), static_cast<f32>(Window::height));
	//	}
	//
	//	Vector2 getFramebufferSize()
	//	{
	//		int width;
	//		int height;
	//		
	//		glfwGetFramebufferSize(getHandle(), &width, &height);
	//		
	//		return Vector2(width, height);
	//	}
	//
	//	bool shouldClose()
	//	{
	//		return glfwWindowShouldClose(getHandle()) == 1;
	//	}
	//
	//	bool isInFocus()
	//	{
	//		return glfwGetWindowAttrib(getHandle(), GLFW_FOCUSED) == 1;
	//	}
	//
	//	bool isIconified()
	//	{
	//		return glfwGetWindowAttrib(getHandle(), GLFW_ICONIFIED) == 1;
	//	}
	//
	//	// callback functions
	//	INTERNAL void resizeCallback(GLFWwindow* window, int width, int height)
	//	{
	//		Window::width = width;
	//		Window::height = height;
	//	}
	//
	//	INTERNAL void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	//	{
	//		glViewport(0, 0, width, height);
	//	}
	//
	//	INTERNAL void errorCallback(int error, const char* description)
	//	{
	//		std::cerr << "[ERROR]";
	//		std::cerr << "GLFW: " << description;
	//		std::cerr << std::endl;
	//	}
	//
	//	INTERNAL void windowRefreshCallback(GLFWwindow* window)
	//	{
	//		//Vector2 fbSize = getFramebufferSize();
	//		//glViewport(0, 0, (GLsizei)fbSize.x, (GLsizei)fbSize.y);
	//
	//		glfwMakeContextCurrent(window);
	//		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//		glfwSwapBuffers(window);
	//	}
	//
	//} // end Window
	//
} // end Dunjun