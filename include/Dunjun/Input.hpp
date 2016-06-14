#ifndef DUNJUN_INPUT_HPP
#define DUNJUN_INPUT_HPP


#include <Dunjun/Window.hpp>


namespace Dunjun
{
	namespace Input
	{
		enum class Key
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

		enum MouseButton 
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

		//struct GamepadAxes // for gamepad
		//{
		//	Vector2 leftThumbStick;
		//	Vector2 rightThumbStick;
		//
		//	f32 leftTrigger;
		//	f32 rightTrigger;
		//};
		//
		//enum class XboxButton
		//{
		//	// values for direct input
		//	DpadUp    = 0,
		//	DpadDown  = 1,
		//	DpadLeft  = 2,
		//	DpadRight = 3,
		//
		//	Start = 4,
		//	Back  = 5,
		//
		//	LeftThumb  = 6,
		//	RightThumb = 7,
		//
		//	LeftShoulder  = 8,
		//	RightShoulder = 9,
		//
		//	A = 10,
		//	B = 11,
		//	X = 12,
		//	Y = 13,
		//
		//	Count = 14,
		//}; // GamepadButtons

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

		enum class GamepadAxis
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

		//enum GamepadId
		//{
		//	Gamepad_1 = 0,
		//	Gamepad_2 = 1,
		//	Gamepad_3 = 2,
		//	Gamepad_4 = 3,
		//
		//	Gamepad_MaxCount = 4,
		//};

		using GamepadButtons = std::vector<b8>;

		enum class CursorMode
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
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		void setCursorMode(CursorMode mode);

		void setStickyKeys(bool stickyKeys);
		void setStickyMouseButtons(bool stickyButtons);

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					KEYBOARD
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool isKeyPressed(Key key);

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					MOUSE
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		Vector2 getCursorPosition();
		Vector2 getCursorPosition(const Window& relativeTo);

		void setCursorPosition(const Vector2& pos);
		void setCursorPosition(const Vector2& pos, const Window& relativeTo);

		bool isMouseButtonPressed(MouseButton button);

		//Vector2 getScrollOffset();

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					TIME
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		//void setTime(Time time);

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					GAMEPAD
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		//void updateGamepads();

		bool isGamepadPresent(u32 gamepadId);
		b8 isGamepadButtonPressed(u32 gamepadId, GamepadButton button);
		f32 getGamepadAxis(u32 gamepadId, GamepadAxis axis);

		std::string getGamepadName(u32 gamepadId);
		void setGamepadVibration(u32 gamepadId, f32 leftMotor, f32 rightMotor);

		std::string getClipboardString();
		void setClipboardString(const std::string& str);


	} // end Input
} // end Dunjun


#endif
