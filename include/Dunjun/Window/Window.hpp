#ifndef  DUNJUN_WINDOW_WINDOW_HPP
#define  DUNJUN_WINDOW_WINDOW_HPP

#include <Dunjun/System/TickCounter.hpp>

namespace Dunjun
{
	struct Var2_int
	{
		Var2_int()
			: x(0)
			, y(0)
		{
		}

		explicit Var2_int(int xy)
			: x(xy)
			, y(xy)
		{
		}

		Var2_int(int x, int y)
			: x(x)
			, y(y)
		{
		}

		Var2_int(int xy[2])
			: x(xy[0])
			, y(xy[1])
		{
		}

		Var2_int(const Var2_int& other) = default; // copy constructor
												 
		// operators
		inline int& operator[](size_t index) { return data[index]; }
		inline const int& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		inline bool operator==(const Var2_int& other) const
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}
		inline bool operator!=(const Var2_int& other) const
		{
			return !operator==(other);
		}

		union // call info as v.x = 1;
		{
			int data[2];
			struct
			{
				int x, y;
			};
			struct
			{
				int r, g;
			};
			struct
			{
				int s, t;
			};
			struct
			{
				int u, v;
			};
		}; // end union
	}; // end Var2_u32

	namespace Style
	{
		enum : u32
		{
			Fullscreen =   1,
			Visible    =   2,
			Hidden 	   =   4,
			Borderless =   8,   
			Resizable  =  16,
			Minimized  =  32,
			Maximized  =  64,
			Windowed   = 128,

			Default = Windowed | Visible | Resizable,
		};

	} // end Style

	struct Event;

	class Window
	{
	public:
		//struct
		//{
		//	// preset window sizes
		//	const int windowSize_854_480[2] = { 854, 480 };
		//	const int windowSize_1366_768[2] = {1366, 768};
		//	const int windowSize_1280_600[2] = { 1280, 600 };
		//
		//	// preset aspect ratios
		//	f32 windowAspect_16_9 = 16.0f / 9.0f;
		//
		//	// window variables
		//	std::string title = "Default Title";
		//	int width = windowSize_854_480[0];
		//	int height = windowSize_854_480[1];
		//	f32 aspectRatio = windowAspect_16_9;
		//	bool isFullscreen = false;
		//
		//}; // end anon struct

		Window();

		explicit Window(const std::string& title, 
						VideoMode mode,
						u32 style = Style::Default,
						const GLContextSettings& context = GLContextSettings());

		virtual ~Window();

		void create(const std::string& title,
					VideoMode mode,
					u32 style = Style::Default,
					const GLContextSettings& context = GLContextSettings());

		void close();
		bool isOpen() const;

		Vector2 getPosition() const;
		Window& setPosition(const Vector2& position);

		Vector2 getSize() const;
		Window& setSize(const Vector2& size);

		const char* getTitle() const;
		Window& setTitle(const char* title);
		Window& setVisible(bool visible);
		Window& setVerticalSyncEnabled(bool enabled);
		Window& setFramerateLimit(u32 limit);

		void display();

		bool pollEvent(Event& event);
		bool waitEvent(Event& event);

		void makeGLContextCurrent() const;

		inline SDL_Window* getSDLHandle() const
		{
			return m_impl;
		}

		Vector2 currentSize = {0, 0};
		f32 currentAspectRatio = 0.0f;
	private:

		SDL_Window* m_impl;
		SDL_GLContext m_glContext;
		Clock m_clock;
		Time m_frameTimeLimit;

	};

} // end Dunjun

#endif
