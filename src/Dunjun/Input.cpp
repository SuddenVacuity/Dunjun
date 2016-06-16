#include <Dunjun/Config.hpp>
#include <Dunjun/Input.hpp>

//#define VC_EXTRALEAN
//#define WIN32_MEAN_AND_LEAN // FIXME: remove Windows.h
//#define WIN32_LEAN_AND_MEAN // FIXME: remove Windows.h
//#include <Windows.h>		// FIXME: remove Windows.h
//
//// this is using Windows 7 - XINPUT9_1_0.LIB
//// change in Linker -> Input -> Additional Dependencies
//#include <xinput.h>
										  
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

		GLOBAL std::array<SDL_GameController*, Gamepad_MaxCount> g_controllerHandles;
	
		void setUp() // set up gamepads
		{
			//for(int i = 0; i < Gamepad_MaxCount; i++) // cycle through and check if they're present
			//{
			//	memset(&g_gamepadStates[i], 0, sizeof(XINPUT_STATE));
			//	if(isGamepadPresent((GamepadId)i))
			//		setGamepadVibration((GamepadId)i, 0, 0); // if isPresent set vibration to 0
			//}

			int maxJoySticks = SDL_NumJoysticks();
			int controllerIndex = 0;
			for (int joystickIndex = 0; joystickIndex < maxJoySticks; joystickIndex++)
			{
				if (!SDL_IsGameController(joystickIndex))
					continue;
				if(controllerIndex >= Gamepad_MaxCount)
					break;

				g_controllerHandles[controllerIndex] = SDL_GameControllerOpen(joystickIndex);
				controllerIndex++;
			}

			setStickyKeys(true); // sticky assumes the button is held until it is checked again
			setStickyMouseButtons(true);
		}

		void cleanup()
		{
			for(int i = 0; i < Gamepad_MaxCount; i++)
			{
				if(isGamepadPresent(i))
					setGamepadVibration(i, 0, 0); // make sure vibration is set to 0 when disconnecting
			}

			for(SDL_GameController* gamepad : g_controllerHandles)
			{
				if(gamepad)
					SDL_GameControllerClose(gamepad);
			}
		}

		//void setInputMode(InputMode mode, int value)
		//{
		//	int m = 0;
		//	if (mode == InputMode::Cursor)
		//		m = GLFW_CURSOR;
		//	if (mode == InputMode::StickyKeys)
		//		m = GLFW_STICKY_KEYS;
		//	if (mode == InputMode::StickyMouseButtons)
		//		m = GLFW_STICKY_MOUSE_BUTTONS;
		//
		//	glfwSetInputMode(Window::getHandle(), m, value);
		//}

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
			//return glfwGetKey(Window::getHandle(), code) == 1;
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

			//glfwGetCursorPos(Window::getHandle(), &x, &y);
			SDL_GetMouseState(&x, &y);

			return Vector2(static_cast<f32>(x),
						   static_cast<f32>(y));
		}

		Vector2 getCursorPosition(const Window& relativeTo)
		{
			s32 x = 0, y = 0;

			//glfwGetCursorPos(Window::getHandle(), &x, &y);
			SDL_GetGlobalMouseState(&x, &y);

			auto pos = relativeTo.getPosition();

			return Vector2(static_cast<f32>(x), static_cast<f32>(y)) - pos;
		}

		void setCursorPosition(const Vector2& pos)
		{
			//glfwSetCursorPos(Window::getHandle(),
			//				 static_cast<f64>(pos.x), 
			//				 static_cast<f64>(pos.y));
			SDL_WarpMouseInWindow(nullptr, pos.x, pos.y);

		}

		void setCursorPosition(const Vector2& pos, const Window& relativeTo)
		{
			//glfwSetCursorPos(Window::getHandle(),
			//				 static_cast<f64>(pos.x), 
			//				 static_cast<f64>(pos.y));
			SDL_WarpMouseInWindow(relativeTo.getNativeHandle(), pos.x, pos.y);

		}

		bool isMouseButtonPressed(MouseButton button)
		{
			return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON((int)button);
			//return glfwGetMouseButton(Window::getHandle(), (int)button) == 1;
			//return false;
		}

		// get scroll wheel movement
		//Vector2 getScrollOffset()
		//{
		//	return Vector2(g_scrollX, g_scrollY);
		//}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					TIME
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		//void setTime(Time time)
		//{
		//	glfwSetTime(time);
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

		//void updateGamepads()
		//{
		//	for(size_t i = 0; i < Gamepad_MaxCount; i++)
		//	{
		//		isGamepadPresent(i);
		//	}
		//}

		bool isGamepadPresent(u32 gamepadId)
		{
			SDL_GameController* gamepad = g_controllerHandles[gamepadId];

			return (gamepad && SDL_GameControllerGetAttached(gamepad));
		}

//
//		GamepadAxes getGamepadAxes(GamepadId gamepadId)
//		{
//			GamepadAxes axes;
//
			//axes.leftTrigger = g_gamepadStates[gamepadId].Gamepad.bLeftTrigger / 255.0f; // convert to correct size
			//axes.rightTrigger = g_gamepadStates[gamepadId].Gamepad.bRightTrigger / 255.0f;
			//
			//axes.leftThumbStick.x = g_gamepadStates[gamepadId].Gamepad.sThumbLX / 32767.0f; // convert to correct size
			//axes.leftThumbStick.y = g_gamepadStates[gamepadId].Gamepad.sThumbLY / 32767.0f;
			//
			//axes.rightThumbStick.x = g_gamepadStates[gamepadId].Gamepad.sThumbRX / 32767.0f; // convert to correct size
			//axes.rightThumbStick.y = g_gamepadStates[gamepadId].Gamepad.sThumbRY / 32767.0f;
//
//			return axes;
//		}
//
//		GamepadButtons getGamepadButtons(GamepadId gamepadId)
//		{
//			GamepadButtons buttons((size_t)XboxButton::Count);
//
			//buttons[(int)XboxButton::DpadUp] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
			//buttons[(int)XboxButton::DpadDown] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
			//buttons[(int)XboxButton::DpadLeft] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
			//buttons[(int)XboxButton::DpadRight] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
			//
			//buttons[(int)XboxButton::Start] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
			//buttons[(int)XboxButton::Back] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
			//
			//buttons[(int)XboxButton::LeftThumb] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
			//buttons[(int)XboxButton::RightThumb] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
			//
			//buttons[(int)XboxButton::LeftShoulder] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
			//buttons[(int)XboxButton::RightShoulder] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
			//
			//buttons[(int)XboxButton::A] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
			//buttons[(int)XboxButton::B] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
			//buttons[(int)XboxButton::X] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
			//buttons[(int)XboxButton::Y] =
			//	(g_gamepadStates[gamepadId].Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
//
//			return buttons;
//		}

		b8 isGamepadButtonPressed(u32 gamepadId, GamepadButton button)
		{
			SDL_GameController* gamepad = g_controllerHandles[gamepadId];

			if(gamepad && SDL_GameControllerGetAttached(gamepad))
				return SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton)button) != 0;

			return false;//getGamepadButtons(gamepadId)[(size_t)button];
		}

		f32 getGamepadAxis(u32 gamepadId, GamepadAxis axis)
		{
			SDL_GameController* gamepad = g_controllerHandles[gamepadId];

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
			return SDL_GameControllerName(g_controllerHandles[gamepadId]);
		}

		void setGamepadVibration(u32 gamepadId, f32 leftMotor, f32 rightMotor)
		{
			//XINPUT_VIBRATION vibration;
			//
			//vibration.wLeftMotorSpeed = static_cast<WORD>(leftMotor * 0xFFFF);
			//vibration.wRightMotorSpeed = static_cast<WORD>(rightMotor * 0xFFFF);
			//
			//XInputSetState((u32)gamepadId, &vibration);
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
