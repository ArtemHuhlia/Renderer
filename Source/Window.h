#pragma once
#include "CoreMinimal.h"
#include "Event/EventManager.h"
#include "Renderer.h"

#include <SDL.h>
#include <SDL_syswm.h>

class Window
{
	SDL_Window*		WindowHandler;
	bool			bIsWindowCLosed;
	DXRenderer		Renderer;
public:
	Window(const char* Title,int Width, int Height, int XPos = SDL_WINDOWPOS_CENTERED, int YPos = SDL_WINDOWPOS_CENTERED);
	Window() = delete;
	void InitDxRenderer();

	FORCEINLINE SDL_Window * GetWindowHandler()
	{
		return WindowHandler;
	}

	bool IsWindowClosed() const;
	void GetWindowWMInfo(HWND & OutputHandler) const;

	void Tick(float InDelta);
public:
	EventManager WindowEventManager;
};