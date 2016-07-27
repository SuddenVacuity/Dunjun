#ifndef DUNJUN_WINDOW_INPUT_HPP
#define DUNJUN_WINDOW_INPUT_HPP


#include <Dunjun/Window/Window.hpp>


namespace Dunjun
{
	namespace Input
	{
		enum class Key : s32
		{
			Unknown = -1,
			A = 0,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			Num0,
			Num1,
			Num2,
			Num3,
			Num4,
			Num5,
			Num6,
			Num7,
			Num8,
			Num9,
			Escape,
			LControl,
			LShift,
			LAlt,
			LSystem,
			RControl,
			RShift,
			RAlt,
			RSystem,
			Menu,
			LBracket,
			RBracket,
			SemiColon,
			Comma,
			Period,
			Apostrophe,
			Slash,
			BackSlash,
			Equal,
			Minus,
			Space,
			Return,
			Backspace,
			Tab,
			GraveAccent,
			World1,
			World2,
			PageUp,
			PageDown,
			End,
			Home,
			Insert,
			Delete,
			Add,
			Subtract,
			Multiply,
			Divide,
			Left,
			Right,
			Up,
			Down,
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			NumpadEnter,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			Pause,

			KeyCount,
		};

		enum class MouseButton
		{
			MouseButton_1 = 1,
			MouseButton_2 = 2,
			MouseButton_3 = 3,
			MouseButton_4 = 4,
			MouseButton_5 = 5,
			MouseButton_6 = 6,
			MouseButton_7 = 7,
			MouseButton_8 = 8,

			//MouseButton_Last = MouseButton_8,

			MouseButton_Left = MouseButton_1,
			MouseButton_Right = MouseButton_2,
			MouseButton_Middle = MouseButton_3,
		};

		enum class GamepadButton : u32
		{
			Invalid		  = -1,	 
								 
			A			  ,	 
			B			  ,	 
			X			  ,	 
			Y			  ,	 
								 
			Back		  ,	 
			Guide		  ,	 
			Start		  ,	 
								 
			LeftStick	  ,	 
			RightStick	  ,	 
								 
			LeftShoulder  ,	 
			RightShoulder ,	 
								 
			DpadUp 		  ,	 
			DpadDown	  ,	 
			DpadLeft 	  ,	 
			DpadRight 	  ,	 

			MaxCount
		};

		enum class GamepadAxis : u32
		{
			Invalid = -1,

			LeftX,
			LeftY,

			RightX,
			RightY,

			LeftTrigger,
			RightTrigger,

			MaxCount
		};

		using GamepadButtons = std::vector<b8>;

		enum class CursorMode : u32
		{
			Normal,
			Hidden,
			Disabled,
		};

		//void scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);

		void setUp();
		void cleanup();

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					INPUT MODES
		)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		//void setCursorMode(CursorMode mode);
		//void setStickyKeys(bool stickyKeys);
		//void setStickyMouseButtons(bool stickyButtons);

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					KEYBOARD
		)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool isKeyPressed(Key key);

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					MOUSE
		)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		Vector2 getCursorPosition();
		Vector2 getCursorPosition(const Window& relativeTo);

		void setCursorPosition(const Vector2& pos);
		void setCursorPosition(const Vector2& pos, const Window& relativeTo);

		bool isMouseButtonPressed(MouseButton button);

		//Vector2 getScrollOffset();

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					GAMEPAD
		)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool isGamepadPresent(u32 gamepadId);
		b8 isGamepadButtonPressed(u32 gamepadId, GamepadButton button);
		f32 getGamepadAxis(u32 gamepadId, GamepadAxis axis);

		String getGamepadName(u32 gamepadId);
		void setGamepadVibration(u32 gamepadId, f32 strength, Time duration = milliseconds(SDL_HAPTIC_INFINITY));

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					CLIPBOARD
		)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool hasClipboardString();
		String getClipboardString();
		void setClipboardString(const String& str);

	} // end Input
} // end Dunjun


#endif
