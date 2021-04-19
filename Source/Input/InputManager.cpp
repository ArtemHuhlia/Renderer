#include "PCH.h"
#include "InputManager.h"

InputManager::KeyInfo InputManager::KeysStates[NUMBER_OF_KEYS];

InputManager::MouseMotionInfo InputManager::MouseInfo;

bool InputManager::IsKeyPressed(SDL_Keycode InKeyCode)
{
	return KeysStates[InKeyCode].bIsKeyDown;
}

InputManager::MouseMotionInfo InputManager::GetMouseMotionInfo()
{
	return MouseInfo;
}

void InputManager::ResetMouseMotionInfo()
{
	MouseInfo.X = 0;
	MouseInfo.Y = 0;
	MouseInfo.RelX = 0;
	MouseInfo.RelY = 0;
}
