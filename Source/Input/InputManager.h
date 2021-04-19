#pragma once
#include "SDL.h"

#define NUMBER_OF_KEYS 512

class InputManager
{
public:
	struct KeyInfo
	{
		bool bIsKeyDown = false;
		bool bIsKeyUp = false;
	};
	struct MouseMotionInfo
	{
		int X = 0, Y = 0;
		int RelX = 0, RelY = 0;
	};

	static KeyInfo KeysStates[NUMBER_OF_KEYS];
	static MouseMotionInfo MouseInfo;
	static bool IsKeyPressed(SDL_Keycode InKeyCode);
	static MouseMotionInfo GetMouseMotionInfo();
	static void ResetMouseMotionInfo();
};


