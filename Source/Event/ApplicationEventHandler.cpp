#include "Event/ApplicationEventHandler.h"

#include "CoreMinimal.h"

bool ApplicationEventHandler::BindOnQuitEvent(QuitEventFunction Function)
{
	CHECK(Function);

	bool bWasBounded = false;
	if (QuitEventBinding)
		bWasBounded = true;

	QuitEventBinding = Function;

	return bWasBounded;
}

bool ApplicationEventHandler::HandleEvent(const union SDL_Event& Event) const 
{
	switch (Event.type)
	{
		case SDL_QUIT: 
		{
			if (QuitEventBinding)
			{
				QuitEventBinding();
				return true;
			}
			return false;
		}
	}
	return false;
}

bool ApplicationEventHandler::IsAppropriateEvent(const SDL_Event& Event) const
{
	return  Event.type >= 0 && Event.type < 0x150;
}
