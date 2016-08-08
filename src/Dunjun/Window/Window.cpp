
#include <Dunjun/Window/Window.hpp>
#include <Dunjun/Window/Event.hpp>

namespace Dunjun
{
	namespace
	{
		const Window* fullscreenWindow = nullptr;

		INTERNAL u32 generateFlags(u32 style)
		{
			u32 flags = 0;

			flags = SDL_WINDOW_OPENGL;

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
		: m_handle(nullptr)
		, m_glContext()
		, m_frameTimeLimit(Time::Zero)
		, m_clock()
	{
	}

	Window::Window(const String& title,
				   VideoMode mode,
				   u32 style,
				   const GLContextSettings& context)
		: m_handle(nullptr)
		, m_glContext()
		, m_frameTimeLimit(Time::Zero)
		, m_clock()
	{
		m_clock.startTime = Time::now();
		create(title, mode, style, context);
	}

	Window::~Window()
	{
		// possibly need to make it this->close();
		close();
	}

	void Window::create(const String& title,
						VideoMode mode,
						u32 style,
						const GLContextSettings& context)
	{
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
				if (!mode.isValid())
				{
					std::cerr << "VideoMode " << mode.width << "x" << mode.height << "x" << mode.bitsPerPixel <<
								 " is not valid." << std::endl;

					mode = VideoMode::getFullscreenModes()[0];

					std::cerr << "Switching to VideoMode " << mode.width << "x" << mode.height << "x" << mode.bitsPerPixel << std::endl;
						
				}

				fullscreenWindow = this;
			}
		}

		u32 windowFlags = generateFlags(style);

		assert(windowFlags && "Window::create windowFlags is nullptr");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context.majorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context.minorVersion);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context.depthBits > 0 ? context.depthBits : 16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context.stencilBits);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, context.antialiasingLevel);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
		//SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, true);

		m_handle = SDL_CreateWindow(cString(title), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			(int)mode.width, (int)mode.height, windowFlags);

		assert(m_handle && "Window::create failed: handle is nullptr");

		m_glContext = SDL_GL_CreateContext(m_handle);

		setVisible(true);
		setFramerateLimit(0);

		Vector2 size = getSize();

		currentSize[0] = size.x;
		currentSize[1] = size.y;
		currentAspectRatio = currentSize[0] / currentSize[1];
	}

	void Window::close()
	{
		SDL_DestroyWindow(m_handle);
		m_handle = nullptr;

		if(this == fullscreenWindow)
			fullscreenWindow = nullptr;
	}

	bool Window::isOpen() const
	{
		return m_handle != nullptr;
	}

	Vector2 Window::getPosition() const
	{
		if(m_handle)
		{
			int x, y;
			SDL_GetWindowPosition(m_handle, &x, &y);

			return {static_cast<f32>(x), 
					static_cast<f32>(y)};
		}
		
		return {0, 0};
	}

	Window& Window::setPosition(const Vector2& position)
	{
		SDL_SetWindowPosition(m_handle, position.x, position.y);

		return *this;
	}

	Vector2 Window::getSize() const
	{
		s32 width, height;
		SDL_GetWindowSize(m_handle, &width, &height);

		return {static_cast<f32>(width), 
				static_cast<f32>(height)};
	}

	Window& Window::setSize(const Vector2& size)
	{
		SDL_SetWindowSize(m_handle, size.x, size.y);

		currentSize[0] = size.x;
		currentSize[1] = size.y;

		if(currentSize[1] == 0)
			currentSize[1] = 1.0f;

		currentAspectRatio = currentSize[0] / currentSize[1];

		return *this;
	}

	String Window::getTitle() const
	{
		return SDL_GetWindowTitle(m_handle);
	}

	Window& Window::setTitle(const String& title)
	{
		SDL_SetWindowTitle(m_handle, cString(title));

		return *this;
	}

	Window& Window::setVisible(bool visible)
	{
		if(visible)
			SDL_ShowWindow(m_handle);
		else
			SDL_HideWindow(m_handle);

		return *this;
	}

	Window& Window::setVerticalSyncEnabled(bool enabled)
	{
		SDL_GL_SetSwapInterval((int)enabled);

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
		SDL_GL_SwapWindow(m_handle);

		if (m_frameTimeLimit != Time::Zero &&
			m_frameTimeLimit > m_clock.getElapsedTime())
		{
			Time::sleep(m_frameTimeLimit - m_clock.getElapsedTime());
			m_clock.restart();
		}

		//if (m_frameTimeLimit != Time::Zero)
		//{
		//	Time::sleep(m_frameTimeLimit - m_clock.getElapsedTime());
		//	m_clock.restart();
		//}
	}

	INTERNAL Input::Key convertFromSDL_ScanCode(u32 code)
	{
			switch (code)														
			{																
				default							:	return Input::Key::Unknown;
				case SDL_SCANCODE_A				:	return Input::Key::A;
				case SDL_SCANCODE_B				:	return Input::Key::B;
				case SDL_SCANCODE_C				:	return Input::Key::C;
				case SDL_SCANCODE_D				:	return Input::Key::D;
				case SDL_SCANCODE_E				:	return Input::Key::E;
				case SDL_SCANCODE_F				:	return Input::Key::F;
				case SDL_SCANCODE_G				:	return Input::Key::G;
				case SDL_SCANCODE_H				:	return Input::Key::H;
				case SDL_SCANCODE_I				:	return Input::Key::I;
				case SDL_SCANCODE_J				:	return Input::Key::J;
				case SDL_SCANCODE_K				:	return Input::Key::K;
				case SDL_SCANCODE_L				:	return Input::Key::L;
				case SDL_SCANCODE_M				:	return Input::Key::M;
				case SDL_SCANCODE_N				:	return Input::Key::N;
				case SDL_SCANCODE_O				:	return Input::Key::O;
				case SDL_SCANCODE_P				:	return Input::Key::P;
				case SDL_SCANCODE_Q				:	return Input::Key::Q;
				case SDL_SCANCODE_R				:	return Input::Key::R;
				case SDL_SCANCODE_S				:	return Input::Key::S;
				case SDL_SCANCODE_T				:	return Input::Key::T;
				case SDL_SCANCODE_U				:	return Input::Key::U;
				case SDL_SCANCODE_V				:	return Input::Key::V;
				case SDL_SCANCODE_W				:	return Input::Key::W;
				case SDL_SCANCODE_X				:	return Input::Key::X;
				case SDL_SCANCODE_Y				:	return Input::Key::Y;
				case SDL_SCANCODE_Z				:	return Input::Key::Z;
				case SDL_SCANCODE_0				:	return Input::Key::Num0;
				case SDL_SCANCODE_1				:	return Input::Key::Num1;
				case SDL_SCANCODE_2				:	return Input::Key::Num2;
				case SDL_SCANCODE_3				:	return Input::Key::Num3;
				case SDL_SCANCODE_4				:	return Input::Key::Num4;
				case SDL_SCANCODE_5				:	return Input::Key::Num5;
				case SDL_SCANCODE_6				:	return Input::Key::Num6;
				case SDL_SCANCODE_7				:	return Input::Key::Num7;
				case SDL_SCANCODE_8				:	return Input::Key::Num8;
				case SDL_SCANCODE_9				:	return Input::Key::Num9;
				case SDL_SCANCODE_ESCAPE		:	return Input::Key::Escape;
				case SDL_SCANCODE_LCTRL			:	return Input::Key::LControl;
				case SDL_SCANCODE_LSHIFT		:	return Input::Key::LShift;
				case SDL_SCANCODE_LALT			:	return Input::Key::LAlt;
				case SDL_SCANCODE_LGUI			:	return Input::Key::LSystem;
				case SDL_SCANCODE_RCTRL			:	return Input::Key::RControl;
				case SDL_SCANCODE_RSHIFT		:	return Input::Key::RShift;
				case SDL_SCANCODE_RALT			:	return Input::Key::RAlt;
				case SDL_SCANCODE_RGUI			:	return Input::Key::RSystem;
				case SDL_SCANCODE_MENU			:	return Input::Key::Menu;
				case SDL_SCANCODE_LEFTBRACKET	:	return Input::Key::LBracket;
				case SDL_SCANCODE_RIGHTBRACKET	:	return Input::Key::RBracket;
				case SDL_SCANCODE_SEMICOLON		:	return Input::Key::SemiColon;
				case SDL_SCANCODE_COMMA			:	return Input::Key::Comma;
				case SDL_SCANCODE_PERIOD		:	return Input::Key::Period;
				case SDL_SCANCODE_APOSTROPHE	:	return Input::Key::Apostrophe;
				case SDL_SCANCODE_SLASH			:	return Input::Key::Slash;
				case SDL_SCANCODE_BACKSLASH		:	return Input::Key::BackSlash;
				case SDL_SCANCODE_EQUALS		:	return Input::Key::Equal;
				case SDL_SCANCODE_MINUS			:	return Input::Key::Minus;
				case SDL_SCANCODE_SPACE			:	return Input::Key::Space;
				case SDL_SCANCODE_RETURN		:	return Input::Key::Return;
				case SDL_SCANCODE_BACKSPACE		:	return Input::Key::Backspace;
				case SDL_SCANCODE_TAB			:	return Input::Key::Tab;
				case SDL_SCANCODE_GRAVE			:	return Input::Key::GraveAccent;
			//	case 0							:	return Input::Key::World1;
			//	case 0							:	return Input::Key::World2;
				case SDL_SCANCODE_PAGEUP		:	return Input::Key::PageUp;
				case SDL_SCANCODE_PAGEDOWN		:	return Input::Key::PageDown;
				case SDL_SCANCODE_END			:	return Input::Key::End;
				case SDL_SCANCODE_HOME			:	return Input::Key::Home;
				case SDL_SCANCODE_INSERT		:	return Input::Key::Insert;
				case SDL_SCANCODE_DELETE		:	return Input::Key::Delete;
				case SDL_SCANCODE_KP_PLUS		:	return Input::Key::Add;
				case SDL_SCANCODE_KP_MINUS		:	return Input::Key::Subtract;
				case SDL_SCANCODE_KP_MULTIPLY	:	return Input::Key::Multiply;
				case SDL_SCANCODE_KP_DIVIDE		:	return Input::Key::Divide;
				case SDL_SCANCODE_LEFT			:	return Input::Key::Left;
				case SDL_SCANCODE_RIGHT			:	return Input::Key::Right;
				case SDL_SCANCODE_UP			:	return Input::Key::Up;
				case SDL_SCANCODE_DOWN			:	return Input::Key::Down;
				case SDL_SCANCODE_KP_0			:	return Input::Key::Numpad0;
				case SDL_SCANCODE_KP_1			:	return Input::Key::Numpad1;
				case SDL_SCANCODE_KP_2			:	return Input::Key::Numpad2;
				case SDL_SCANCODE_KP_3			:	return Input::Key::Numpad3;
				case SDL_SCANCODE_KP_4			:	return Input::Key::Numpad4;
				case SDL_SCANCODE_KP_5			:	return Input::Key::Numpad5;
				case SDL_SCANCODE_KP_6			:	return Input::Key::Numpad6;
				case SDL_SCANCODE_KP_7			:	return Input::Key::Numpad7;
				case SDL_SCANCODE_KP_8			:	return Input::Key::Numpad8;
				case SDL_SCANCODE_KP_9			:	return Input::Key::Numpad9;
				case SDL_SCANCODE_KP_ENTER		:	return Input::Key::NumpadEnter;
				case SDL_SCANCODE_F1			:	return Input::Key::F1;
				case SDL_SCANCODE_F2			:	return Input::Key::F2;
				case SDL_SCANCODE_F3			:	return Input::Key::F3;
				case SDL_SCANCODE_F4			:	return Input::Key::F4;
				case SDL_SCANCODE_F5			:	return Input::Key::F5;
				case SDL_SCANCODE_F6			:	return Input::Key::F6;
				case SDL_SCANCODE_F7			:	return Input::Key::F7;
				case SDL_SCANCODE_F8			:	return Input::Key::F8;
				case SDL_SCANCODE_F9			:	return Input::Key::F9;
				case SDL_SCANCODE_F10			:	return Input::Key::F10;
				case SDL_SCANCODE_F11			:	return Input::Key::F11;
				case SDL_SCANCODE_F12			:	return Input::Key::F12;
				case SDL_SCANCODE_F13			:	return Input::Key::F13;
				case SDL_SCANCODE_F14			:	return Input::Key::F14;
				case SDL_SCANCODE_F15			:	return Input::Key::F15;
				case SDL_SCANCODE_PAUSE			:	return Input::Key::Pause;
			}
			return Input::Key::Unknown;
	}



	INTERNAL void convertEvent(SDL_Event& e, Event& event)
	{
		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					WINDOW EVENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		switch(e.type)
		{
		default: event.type = Event::Unknown; return;
		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				event.type = Event::Resized;
				event.size.width = e.window.data1;
				event.size.height = e.window.data2; 
				//std::cout << "Event - Window Resized" << std::endl;
			}
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				event.type = Event::Closed;
				std::cout << "Event - Window Closed" << std::endl;
			}
			if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
			{
				event.type = Event::GainedFocus;
				std::cout << "Event - Window Gained Focus" << std::endl;
			}
			if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
			{
				event.type = Event::LostFocus;
				std::cout << "Event - Window Lost Focus" << std::endl;
			}
			if (e.window.event == SDL_WINDOWEVENT_MOVED)
			{
				event.type = Event::Moved;
				event.move.x = e.window.data1;
				event.move.y = e.window.data2;
				std::cout << "Event - Window Moved" << std::endl;
			}
			return;
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					MOUSE EVENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		case SDL_MOUSEMOTION:
		{
			event.type = Event::MouseMoved;
			event.mouseMove.x = e.motion.x;
			event.mouseMove.y = e.motion.y;
			//std::cout << "Event - Mouse Moved" << std::endl;
			return;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			event.type = Event::MouseButtonPressed;
			event.mouseButton.button = (Input::MouseButton)e.button.button;
			event.mouseButton.clicks = e.button.clicks;

			event.mouseMove.x = e.button.x;
			event.mouseMove.y = e.button.y;
			std::cout << "Event - Mouse Button Pressed: " << (int)e.button.button << std::endl;
			return;
		}

		case SDL_MOUSEBUTTONUP:
		{
			event.type = Event::MouseButtonReleased;
			event.mouseButton.button = (Input::MouseButton)e.button.button;
			event.mouseButton.clicks = e.button.clicks;

			event.mouseMove.x = e.button.x;
			event.mouseMove.y = e.button.y;
			std::cout << "Event - Mouse Button Released: " << (int)e.button.button << std::endl;
			return;
		}

		case SDL_MOUSEWHEEL:
		{
			event.type = Event::MouseWheelScrolled;
			event.mouseScrollWheel.deltaX = e.wheel.x;
			event.mouseScrollWheel.deltaY = e.wheel.y;
			std::cout << "Event - Mouse Wheel Scrolled:" << std::endl;
			return;
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					KEYBOARD EVENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		case SDL_KEYDOWN:
		{
			event.type = Event::KeyPressed;
			event.key.code = convertFromSDL_ScanCode(e.key.keysym.scancode);
			u16 mod = e.key.keysym.mod;

			if (mod & KMOD_ALT)
				event.key.alt = true;
			if (mod & KMOD_CTRL)
				event.key.ctrl = true;
			if (mod & KMOD_SHIFT)
				event.key.shift = true;
			if (mod & KMOD_GUI)
				event.key.system = true;
			if (mod & KMOD_CAPS)
				event.key.capsLock = true;
			if (mod & KMOD_NUM)
				event.key.numLock = true;
			std::cout << "Event - Key Pressed:" << (int)e.button.button << std::endl;
			return;
		}

		case SDL_KEYUP:
		{
			event.type = Event::KeyReleased;
			event.key.code = convertFromSDL_ScanCode(e.key.keysym.scancode);
			u16 mod = e.key.keysym.mod;

			if (mod & KMOD_ALT)
				event.key.alt = true;
			if (mod & KMOD_CTRL)
				event.key.ctrl = true;
			if (mod & KMOD_SHIFT)
				event.key.shift = true;
			if (mod & KMOD_GUI)
				event.key.system = true;
			if (mod & KMOD_CAPS)
				event.key.capsLock = true;
			if (mod & KMOD_NUM)
				event.key.numLock = true;
			std::cout << "Event - Key Released: " << (int)e.button.button << std::endl;
			return;
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					GAMEPAD EVENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		case SDL_CONTROLLERBUTTONDOWN:
		{
			event.type = Event::GamepadButtonPressed;
			event.gamepadButton.id = e.button.which;
			event.gamepadButton.button = (Input::GamepadButton)e.cbutton.button;
			std::cout << "Event - Gamepad Button Pressed:" << (int)e.cbutton.button << std::endl;
			return;
		}

		case SDL_CONTROLLERBUTTONUP:
		{
			event.type = Event::GamepadButtonReleased;
			event.gamepadButton.id = e.button.which;
			event.gamepadButton.button = (Input::GamepadButton)e.cbutton.button;
			std::cout << "Event - Gamepad Button Released:" << (int)e.cbutton.button << std::endl;
			return;
		}

		case SDL_CONTROLLERDEVICEADDED:
		{
			event.type = Event::GamepadAdded;
			event.gamepad.id = e.cdevice.which;
			std::cout << "Event - Gamepad Device Added: " << (int)e.cdevice.which << std::endl;
			return;
		}

		case SDL_CONTROLLERDEVICEREMOVED:
		{
			event.type = Event::GamepadRemoved;
			event.gamepad.id = e.cdevice.which;
			std::cout << "Event - Gamepad Device Removed: " << (int)e.cdevice.which << std::endl;
			return;
		}

		case SDL_CONTROLLERDEVICEREMAPPED:
		{
			event.type = Event::GamepadRemapped;
			event.gamepad.id = e.cdevice.which;
			std::cout << "Event - Gamepad Device Remapped: " << (int)e.cdevice.which << std::endl;
			return;
		}
		
		case SDL_CONTROLLERAXISMOTION:
		{
			event.type = Event::GamepadAxisChanged;
			event.gamepadAxis.id = e.caxis.which;
			event.gamepadAxis.axis = (Input::GamepadAxis)e.caxis.axis;
			
			s16 value = e.caxis.value;
			// account for signed 0
			if (value >= 0)
				event.gamepadAxis.value =  static_cast<f32>(value) / 32767.0f;
			else
				event.gamepadAxis.value = static_cast<f32>(value) / 32768.0f;

			//if(Math::abs(event.gamepadAxis.value) > 0.2f)
			//	std::cout << "Event - Gamepad " << (int)event.gamepadAxis.id << " Axis " << (int)e.caxis.axis << " Changed:  " << (f32)event.gamepadAxis.value << std::endl;
			return;
		}
		} // end switch(e.type)

		return;
	} // end converEvent()

	bool Window::pollEvent(Event& event)
	{
		SDL_Event e;
		if(SDL_PollEvent(&e))
		{
			convertEvent(e, event);
			return true;
		}

		return false;
	}

	bool Window::waitEvent(Event& event)
	{
		SDL_Event e;
		if (SDL_WaitEvent(&e))
		{
			convertEvent(e, event);
			return true;
		}

		return false;
	}


} // end Dunjun