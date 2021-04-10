#include "PCH.h"
#include "EventManager.h"

#include "SDL.h"

void EventManager::PollEvents()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (KeybordEvents.IsAppropriateEvent(Event))
		{
			if (KeybordEvents.HandleEvent(Event))
				continue;
		}
		if (MouseEvents.IsAppropriateEvent(Event))
		{
			if (MouseEvents.HandleEvent(Event))
				continue;
		}
		if (ApplicationEvents.IsAppropriateEvent(Event))
		{
			if (ApplicationEvents.HandleEvent(Event))
				continue;
		}
	}
}

