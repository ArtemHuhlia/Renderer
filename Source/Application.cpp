#include "Window.h"
#include "Input/InputManager.h"

int main(int, char**)
{
	Window RendererWindow("Renderer", 1280.f, 720.f);
	RendererWindow.WindowEventManager.KeybordEvents.BindOnAnyKeyDownEvent([](SDL_Keycode Keycode)
	{
		if (Keycode > 0 && Keycode < NUMBER_OF_KEYS)
			InputManager::KeysStates[Keycode].bIsKeyDown = true;
	}
	);
	RendererWindow.WindowEventManager.KeybordEvents.BindOnAnyKeyUpEvent([](SDL_Keycode Keycode)
	{
		if (Keycode > 0 && Keycode < NUMBER_OF_KEYS)
			InputManager::KeysStates[Keycode].bIsKeyDown = false;
	}
	);
	RendererWindow.WindowEventManager.MouseEvents.BindOnMouseMotionEvent([](int32_t X, int32_t Y, int32_t RelX, int32_t RelY) {
		InputManager::MouseInfo.X = X;
		InputManager::MouseInfo.Y = Y;
		InputManager::MouseInfo.RelX = RelX;
		InputManager::MouseInfo.RelY = RelY;
	});

	float CurrentFrameTimeMs = 0.f;
	float LastFrameTimeMs = 0.f;
	float DeltaTimeMs = 0.f;

	while(!RendererWindow.IsWindowClosed())
	{
		CurrentFrameTimeMs = SDL_GetTicks();
		DeltaTimeMs = CurrentFrameTimeMs - LastFrameTimeMs;
		LastFrameTimeMs = CurrentFrameTimeMs;

		RendererWindow.WindowEventManager.PollEvents();

		RendererWindow.Tick(DeltaTimeMs);
	}
	return 0;
}

