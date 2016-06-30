
#include <Dunjun/Window/Input.hpp>
								  
//#include <SDL/SDL_keyboard.h>		  // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_keycode.h>		  // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_mouse.h>		  // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_clipboard.h>	  // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_events.h>		  // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_gamecontroller.h> // included in Windows.hpp (SDL.h)
//#include <SDL/SDL_joystick.h>		  // included in Windows.hpp (SDL.h)

namespace Dunjun
{
	namespace Input
	{
		GLOBAL const size_t Gamepad_MaxCount = 4;

		//GLOBAL std::array<XINPUT_STATE, Gamepad_MaxCount> g_gamepadStates;
		GLOBAL f64 g_scrollX = 0;
		GLOBAL f64 g_scrollY = 0;

		GLOBAL std::array<SDL_GameController*, Gamepad_MaxCount> g_gamepadHandles;
		GLOBAL std::array<SDL_Haptic*, Gamepad_MaxCount> g_rumbleHandles;
	
		void setUp() // set up gamepads
		{
			int maxJoySticks = SDL_NumJoysticks();
			int gamepadIndex = 0;

			for (int joystickIndex = 0; joystickIndex < maxJoySticks; joystickIndex++)
			{
				if (!SDL_IsGameController(joystickIndex))
					continue;
				if(gamepadIndex >= Gamepad_MaxCount)
					break;

				std::cout << "Adding gamepad: " << gamepadIndex << ::std::endl;

				g_gamepadHandles[gamepadIndex] = SDL_GameControllerOpen(joystickIndex);

				SDL_Joystick* joystickHandle = SDL_GameControllerGetJoystick(g_gamepadHandles[gamepadIndex]);

				g_rumbleHandles[gamepadIndex] = SDL_HapticOpenFromJoystick(joystickHandle);

				if (SDL_HapticRumbleSupported(g_rumbleHandles[gamepadIndex]) != true)
				{
					std::cout << "Input::setUp() " << "Gamepad " << gamepadIndex <<
						" -Rumble device not supported-SDL(" <<
						SDL_GetError() << ")" << std::endl;
				}
				if (SDL_HapticRumbleInit(g_rumbleHandles[gamepadIndex]) == -1);
				{
					std::cout << "Input::setUp() " << "Gamepad " << gamepadIndex <<
						" -Rumble initialization failed-SDL(" <<
						SDL_GetError() << ")" << std::endl;

					SDL_HapticClose(g_rumbleHandles[gamepadIndex]);
					g_rumbleHandles[gamepadIndex] = nullptr;
				}

				std::cout << "Gamepad " << gamepadIndex << " Added\n" << std::endl;

				gamepadIndex++;
			}

			setStickyKeys(true); // sticky assumes the button is held until it is checked again
			setStickyMouseButtons(true);
		}

		void cleanup()
		{
			for(int i = 0; i < Gamepad_MaxCount; i++)
			{
				if(isGamepadPresent(i))
					setGamepadVibration(i, 0, Time::Zero); // make sure vibration is set to 0 when disconnecting
			}

			for(SDL_GameController* gamepad : g_gamepadHandles)
			{
				if(gamepad)
					SDL_GameControllerClose(gamepad);
			}
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					INPUT MODES
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		void setCursorMode(CursorMode mode)
		{
			//if (mode == CursorMode::Normal)
			//	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//if (mode == CursorMode::Hidden)
			//	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			//if (mode == CursorMode::Disabled)
			//	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		void setStickyKeys(bool stickyKeys)
		{
			//glfwSetInputMode(Window::getHandle(), GLFW_STICKY_KEYS, stickyKeys);
		}

		void setStickyMouseButtons(bool stickyButtons)
		{
			//glfwSetInputMode(Window::getHandle(), GLFW_STICKY_MOUSE_BUTTONS, stickyButtons);
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					KEYBOARD
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		// get keyboard key pressed
		bool isKeyPressed(Key key)
		{
			int code = 0;

			// check keyboard key
			switch (key)														
			{																
				default 				  : code = 0;								break;
				case Key::A				  : code = SDL_SCANCODE_A;					break;
				case Key::B				  : code = SDL_SCANCODE_B;					break;
				case Key::C				  : code = SDL_SCANCODE_C;					break;
				case Key::D				  : code = SDL_SCANCODE_D;					break;
				case Key::E				  : code = SDL_SCANCODE_E;					break;
				case Key::F				  : code = SDL_SCANCODE_F;					break;
				case Key::G				  : code = SDL_SCANCODE_G;					break;
				case Key::H				  : code = SDL_SCANCODE_H;					break;
				case Key::I				  : code = SDL_SCANCODE_I;					break;
				case Key::J				  : code = SDL_SCANCODE_J;					break;
				case Key::K				  : code = SDL_SCANCODE_K;					break;
				case Key::L				  : code = SDL_SCANCODE_L;					break;
				case Key::M				  : code = SDL_SCANCODE_M;					break;
				case Key::N				  : code = SDL_SCANCODE_N;					break;
				case Key::O				  : code = SDL_SCANCODE_O;					break;
				case Key::P				  : code = SDL_SCANCODE_P;					break;
				case Key::Q				  : code = SDL_SCANCODE_Q;					break;
				case Key::R				  : code = SDL_SCANCODE_R;					break;
				case Key::S				  : code = SDL_SCANCODE_S;					break;
				case Key::T				  : code = SDL_SCANCODE_T;					break;
				case Key::U				  : code = SDL_SCANCODE_U;					break;
				case Key::V				  : code = SDL_SCANCODE_V;					break;
				case Key::W				  : code = SDL_SCANCODE_W;					break;
				case Key::X				  : code = SDL_SCANCODE_X;					break;
				case Key::Y				  : code = SDL_SCANCODE_Y;					break;
				case Key::Z				  : code = SDL_SCANCODE_Z;					break;
				case Key::Num0			  : code = SDL_SCANCODE_0;					break;
				case Key::Num1			  : code = SDL_SCANCODE_1;					break;
				case Key::Num2			  : code = SDL_SCANCODE_2;					break;
				case Key::Num3			  : code = SDL_SCANCODE_3;					break;
				case Key::Num4			  : code = SDL_SCANCODE_4;					break;
				case Key::Num5			  : code = SDL_SCANCODE_5;					break;
				case Key::Num6			  : code = SDL_SCANCODE_6;					break;
				case Key::Num7			  : code = SDL_SCANCODE_7;					break;
				case Key::Num8			  : code = SDL_SCANCODE_8;					break;
				case Key::Num9			  : code = SDL_SCANCODE_9;					break;
				case Key::Escape		  : code = SDL_SCANCODE_ESCAPE;				break;
				case Key::LControl		  : code = SDL_SCANCODE_LCTRL;				break;
				case Key::LShift		  : code = SDL_SCANCODE_LSHIFT;				break;
				case Key::LAlt			  : code = SDL_SCANCODE_LALT;				break;
				case Key::LSystem		  : code = SDL_SCANCODE_LGUI;				break;
				case Key::RControl		  : code = SDL_SCANCODE_RCTRL;				break;
				case Key::RShift		  : code = SDL_SCANCODE_RSHIFT;				break;
				case Key::RAlt			  : code = SDL_SCANCODE_RALT;				break;
				case Key::RSystem		  : code = SDL_SCANCODE_RGUI;				break;
				case Key::Menu			  : code = SDL_SCANCODE_MENU;				break;
				case Key::LBracket		  : code = SDL_SCANCODE_LEFTBRACKET;		break;
				case Key::RBracket		  : code = SDL_SCANCODE_RIGHTBRACKET;		break;
				case Key::SemiColon		  : code = SDL_SCANCODE_SEMICOLON;			break;
				case Key::Comma			  : code = SDL_SCANCODE_COMMA;				break;
				case Key::Period		  : code = SDL_SCANCODE_PERIOD;				break;
				case Key::Apostrophe	  : code = SDL_SCANCODE_APOSTROPHE;			break;
				case Key::Slash			  : code = SDL_SCANCODE_SLASH;				break;
				case Key::BackSlash		  : code = SDL_SCANCODE_BACKSLASH;			break;
				case Key::Equal			  : code = SDL_SCANCODE_EQUALS;				break;
				case Key::Minus			  : code = SDL_SCANCODE_MINUS;				break;
				case Key::Space			  : code = SDL_SCANCODE_SPACE;				break;
				case Key::Return		  : code = SDL_SCANCODE_RETURN;				break;
				case Key::Backspace		  : code = SDL_SCANCODE_BACKSPACE;			break;
				case Key::Tab			  : code = SDL_SCANCODE_TAB;				break;
				case Key::GraveAccent	  : code = SDL_SCANCODE_GRAVE;				break;
				case Key::World1		  : code = 0;								break;
				case Key::World2		  : code = 0;								break;
				case Key::PageUp		  : code = SDL_SCANCODE_PAGEUP;				break;
				case Key::PageDown		  : code = SDL_SCANCODE_PAGEDOWN;			break;
				case Key::End			  : code = SDL_SCANCODE_END;				break;
				case Key::Home			  : code = SDL_SCANCODE_HOME;				break;
				case Key::Insert		  : code = SDL_SCANCODE_INSERT;				break;
				case Key::Delete		  : code = SDL_SCANCODE_DELETE;				break;
				case Key::Add			  : code = SDL_SCANCODE_KP_PLUS;			break;
				case Key::Subtract		  : code = SDL_SCANCODE_KP_MINUS;			break;
				case Key::Multiply		  : code = SDL_SCANCODE_KP_MULTIPLY;		break;
				case Key::Divide		  : code = SDL_SCANCODE_KP_DIVIDE;			break;
				case Key::Left			  : code = SDL_SCANCODE_LEFT;				break;
				case Key::Right			  : code = SDL_SCANCODE_RIGHT;				break;
				case Key::Up			  : code = SDL_SCANCODE_UP;					break;
				case Key::Down			  : code = SDL_SCANCODE_DOWN;				break;
				case Key::Numpad0		  : code = SDL_SCANCODE_KP_0;				break;
				case Key::Numpad1		  : code = SDL_SCANCODE_KP_1;				break;
				case Key::Numpad2		  : code = SDL_SCANCODE_KP_2;				break;
				case Key::Numpad3		  : code = SDL_SCANCODE_KP_3;				break;
				case Key::Numpad4		  : code = SDL_SCANCODE_KP_4;				break;
				case Key::Numpad5		  : code = SDL_SCANCODE_KP_5;				break;
				case Key::Numpad6		  : code = SDL_SCANCODE_KP_6;				break;
				case Key::Numpad7		  : code = SDL_SCANCODE_KP_7;				break;
				case Key::Numpad8		  : code = SDL_SCANCODE_KP_8;				break;
				case Key::Numpad9		  : code = SDL_SCANCODE_KP_9;				break;
				case Key::NumpadEnter	  : code = SDL_SCANCODE_KP_ENTER;			break;
				case Key::F1			  : code = SDL_SCANCODE_F1;					break;
				case Key::F2			  : code = SDL_SCANCODE_F2;					break;
				case Key::F3			  : code = SDL_SCANCODE_F3;					break;
				case Key::F4			  : code = SDL_SCANCODE_F4;					break;
				case Key::F5			  : code = SDL_SCANCODE_F5;					break;
				case Key::F6			  : code = SDL_SCANCODE_F6;					break;
				case Key::F7			  : code = SDL_SCANCODE_F7;					break;
				case Key::F8			  : code = SDL_SCANCODE_F8;					break;
				case Key::F9			  : code = SDL_SCANCODE_F9;					break;
				case Key::F10			  : code = SDL_SCANCODE_F10;				break;
				case Key::F11			  : code = SDL_SCANCODE_F11;				break;
				case Key::F12			  : code = SDL_SCANCODE_F12;				break;
				case Key::F13			  : code = SDL_SCANCODE_F13;				break;
				case Key::F14			  : code = SDL_SCANCODE_F14;				break;
				case Key::F15			  : code = SDL_SCANCODE_F15;				break;
				case Key::Pause			  : code = SDL_SCANCODE_PAUSE;				break;

			}

			if(code == 0)
				return false;

			const u8* state = SDL_GetKeyboardState(nullptr);
 
			return state[code];
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					MOUSE
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		Vector2 getCursorPosition()
		{
			s32 x = 0, y = 0;

			SDL_GetMouseState(&x, &y);

			return {static_cast<f32>(x),
					static_cast<f32>(y)};
		}

		Vector2 getCursorPosition(const Window& relativeTo)
		{
			s32 x = 0, y = 0;

			SDL_GetGlobalMouseState(&x, &y);

			Vector2 pos = relativeTo.getPosition();

			return {static_cast<f32>(x) - pos.x, 
					static_cast<f32>(y) - pos.y};
		}

		void setCursorPosition(const Vector2& pos)
		{
			SDL_WarpMouseInWindow(nullptr, pos.x, pos.y);
		}

		void setCursorPosition(const Vector2& pos, const Window& relativeTo)
		{
			SDL_WarpMouseInWindow(relativeTo.handle, pos.x, pos.y);
		}

		bool isMouseButtonPressed(MouseButton button)
		{
			return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON((int)button);
		}

		// get scroll wheel movement
		//Vector2 getScrollOffset()
		//{
		//	return Vector2(g_scrollX, g_scrollY);
		//}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					GAMEPAD
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool isGamepadPresent(u32 gamepadId)
		{
			SDL_GameController* gamepad = g_gamepadHandles[gamepadId];

			return (gamepad && SDL_GameControllerGetAttached(gamepad));
		}

		b8 isGamepadButtonPressed(u32 gamepadId, GamepadButton button)
		{
			SDL_GameController* gamepad = g_gamepadHandles[gamepadId];

			if(gamepad && SDL_GameControllerGetAttached(gamepad))
				return SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton)button) != 0;

			return false;
		}

		f32 getGamepadAxis(u32 gamepadId, GamepadAxis axis)
		{
			SDL_GameController* gamepad = g_gamepadHandles[gamepadId];

			if (gamepad && SDL_GameControllerGetAttached(gamepad))
			{
				s16 value = SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis)axis);

			// account for signed 0
				if(value >= 0)
					return static_cast<f32>(value) / 32767.0f;

				return static_cast<f32>(value) / 32768.0f;
			}

			return 0;
		}

		std::string getGamepadName(u32 gamepadId)
		{
			return SDL_GameControllerName(g_gamepadHandles[gamepadId]);
		}

		void setGamepadVibration(u32 gamepadId, f32 strength, Time duration)
		{
			strength = Math::clamp(strength, 0.0f, 1.0f);
			if (g_rumbleHandles[gamepadId])
			{
				SDL_HapticRumblePlay(g_rumbleHandles[gamepadId], strength, duration.asMilliseconds());
			}
		}

		std::string getClipboardString()
		{
			//return glfwGetClipboardString(Window::getHandle());
			return "";
		}

		void setClipboardString(const std::string& str)
		{
			//glfwSetClipboardString(Window::getHandle(), str.c_str());
		}

	} // end Input
} // end Dunjun
