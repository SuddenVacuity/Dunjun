#ifndef DUNJUN_INPUT_HPP
#define DUNJUN_INPUT_HPP

#include <Dunjun/Game.hpp>

#define WINDOWS_MEAN_AND_LEAN
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

// this is using Windows 7 - XINPUT9_1_0.LIB
// change in Linker -> Input -> Additional Dependencies
#include <xinput.h>

namespace Dunjun
{
	namespace Input
	{
		using Key = int;
		using MouseButton = int;

		enum KeyState : u32 // for keyboard
		{
			Release = 0,
			Press = 1,
		};
		
		enum class InputMode : u32 // for keyboard/mouse
		{
			Cursor, // GLFW values
			StickyKeys,
			StickyMouseButtons,
		};

		enum GamepadId
		{
			Gamepad_1 = 0,
			Gamepad_2 = 1,
			Gamepad_3 = 2,
			Gamepad_4 = 3,

			Gamepad_maxCount = 4,
		};

		struct GamepadAxes // for gamepad
		{
			Vector2 leftThumbStick;
			Vector2 rightThumbStick;

			f32 leftTrigger;
			f32 rightTrigger;
		};

		enum class XboxButton
		{
			// values for direct input
			DpadUp = 0,
			DpadDown = 1,
			DpadLeft = 2,
			DpadRight = 3,

			Start = 4,
			Back = 5,

			LeftThumb = 6,
			RightThumb = 7,

			LeftShoulder = 8,
			RightShoulder = 9,

			A = 10,
			B = 11,
			X = 12,
			Y = 13,

			Count = 14,
		}; // GamepadButtons

		using GamepadButtons = std::vector<b8>;

		void setInputMode(InputMode mode, int value);

		KeyState getKey(Key key);

		Vector2 getCursorPosition();
		void setCursorPosition(const Vector2& pos);

		f64 getTime();
		void setTime(f64 time);

		void updateGamepads();

		bool isGamepadPresent(GamepadId gamepadId);
		GamepadAxes getGamepadAxes(GamepadId gamepadId);
		GamepadButtons getGamepadButtons(GamepadId gamepadId);
		std::string getGamepadName(GamepadId gamepadId);



	} // end Input
} // end Dunjun

#endif
