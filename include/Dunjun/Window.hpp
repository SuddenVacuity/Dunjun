#ifndef  DUNJUN_WINDOW_HPP
#define  DUNJUN_WINDOW_HPP

#include <Dunjun/TickCounter.hpp>

//#include <GLFW/glfw3.h>
#include <SDL/SDL.h>

//struct GLFWwindow;
//struct GLFWmonitor;

namespace Dunjun
{
	struct Var2_u32
	{
		Var2_u32();

		explicit Var2_u32(u32 xy)
			: x(xy)
			, y(xy)
		{
		}

		Var2_u32(u32 x, u32 y)
			: x(x)
			, y(y)
		{
		}

		Var2_u32(u32 xy[2])
			: x(xy[0])
			, y(xy[1])
		{
		}

		Var2_u32(const Var2_u32& other) = default; // copy constructor
												 
		// operators
		inline u32& operator[](size_t index) { return data[index]; }
		inline const u32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		inline bool operator==(const Var2_u32& other) const
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}
		inline bool operator!=(const Var2_u32& other) const
		{
			return !operator==(other);
		}

		union // call info as v.x = 1;
		{
			u32 data[2];
			struct
			{
				u32 x, y;
			};
			struct
			{
				u32 r, g;
			};
			struct
			{
				u32 s, t;
			};
			struct
			{
				u32 u, v;
			};
		}; // end union
	}; // end Var2_u32

	namespace Style
	{
		enum : u32
		{
			Borderless    =   1,
			Windowed	  =   2,
			Fullscreen	  =   4,
			Visible		  =   8,
			Hidden		  =  16,
			Minimized	  =  32,
			Maximized	  =  64,
			Resizable     = 128,

			Default = Windowed | Visible | Resizable,
		};

	} // end Style

	class Window
	{
	public:
		Window();

		explicit Window(const std::string& title, 
						const u32& width, const u32& height, 
						u32 style);

		virtual ~Window();

		void create(const std::string& title,
					const u32& width, const u32& height,
					u32 style);

		void close();
		bool isOpen() const;

		Vector2 getPosition() const;
		Window& setPosition(const Vector2& position);

		Var2_u32 getSize() const;
		Window& setSize(u32 width, u32 height);

		Window& setTitle(std::string& title);
		Window& setVisible(bool visible);
		Window& setVerticalSyncEnabled(bool enabled);
		Window& setFramerateLimit(u32 limit);

		void display();

	private:
		void init();

		SDL_Window* m_impl;
		SDL_GLContext m_context;
		Clock m_clock;
		Time m_frameTimeLimit;
		Var2_u32 m_size;





	};



	//namespace Window
	//{
	//	namespace
	//	{
	//		// preset window sizes
	//		const int windowSize_854_480[2] = { 854, 480 };
	//		const int windowSize_1366_768[2] = {1366, 768};
	//		const int windowSize_1280_600[2] = { 1280, 600 };
	//
	//		// preset aspect ratios
	//		f32 windowAspect_16_9 = 16.0f / 9.0f;
	//
	//		// window variables
	//		int width = windowSize_854_480[0];
	//		int height = windowSize_854_480[1];
	//		f32 aspectRatio = windowAspect_16_9;
	//		bool isFullscreen = false;
	//
	//	} // end anon namespace
	//
	//	extern GLFWwindow* ptr;
	//
	//	GLFWwindow* getHandle();
	//	void setHandle(GLFWwindow* w);
	//
	//	//extern bool getIsFullscreen();
	//	//void setFullscreen(bool fullscreen, GLFWmonitor* monitor);
	//
	//	bool init();
	//	void cleanup();
	//
	//	void destroyWindow();
	//	void destroyWindow(GLFWwindow* w);
	//
	//	GLFWwindow* createWindow(GLFWmonitor* monitor);
	//	GLFWwindow* createWindow(GLFWmonitor* monitor, u32 width, u32 height);
	//
	//	void makeContextCurrent();
	//	void swapInterval(int i);
	//
	//	void swapBuffers();
	//	void pollEvents();
	//
	//	void setTitle(const std::string& title);
	//
	//	//bool isFullscreen();
	//	//void setFullscreen(bool fullscreen);
	//
	//	Vector2 getWindowSize();
	//	Vector2 getFramebufferSize();
	//
	//	bool shouldClose();
	//	bool isInFocus();
	//	bool isIconified();
	//
	//	//void setIcon(const Image& image);
	//	//void setFramerateLimit(u32 limit);
	//	//void setVerticalSyncEnabled(bool enabled);
	//	//bool hasFocus();
	//	//
	//	//bool pollEvent(Event& event);
	//	//bool waitEvent(Event& event);
	//
	//} // end Window
} // end Dunjun

#endif
