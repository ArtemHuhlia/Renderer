#include "PCH.h"
#include "Event/KeyEventHandler.h"
#include "CoreMinimal.h"

bool KeyEventHandler::BindOnKeyDownEvent(SDL_Keycode KeyCode, KeyDownEventFunction Function)
{
	CHECK(Function);
	return KeyDownEventBindings.insert({ KeyCode, Function }).second;
}

bool KeyEventHandler::BindOnKeyUpEvent(SDL_Keycode KeyCode, KeyUpEventFunction Function)
{
	CHECK(Function);
	return KeyUpEventBindings.insert({ KeyCode, Function }).second;
}

bool KeyEventHandler::BindOnAnyKeyDownEvent(AnyKeyDownEventFunction Function)
{
	CHECK(Function);
	bool bWasBounded = false;
	if (AnyKeyDownEventBinding)
		bWasBounded = true;

	AnyKeyDownEventBinding = Function;

	return bWasBounded;
}

bool KeyEventHandler::BindOnAnyKeyUpEvent(AnyKeyUpEventFunction Function)
{
	CHECK(Function);
	bool bWasBounded = false;
	if (AnyKeyUpEventBinding)
		bWasBounded = true;

	AnyKeyUpEventBinding = Function;

	return bWasBounded;
}

bool KeyEventHandler::HandleEvent(const SDL_Event& Event) const
{
	bool Result = false;
	switch (Event.type)
	{
	 	case SDL_KEYDOWN:
	 	{

			if (AnyKeyDownEventBinding)
			{
				AnyKeyDownEventBinding(Event.key.keysym.sym);
				Result |= true;
			}

	 		auto It = KeyDownEventBindings.find(Event.key.keysym.sym);
	 		if (It != KeyDownEventBindings.end() && It->second)
	 		{
	 			It->second();
				Result |= true;
	 		}
			Result |= false;
			break;
	 	}
	 	case SDL_KEYUP:
	 	{
			if (AnyKeyUpEventBinding)
			{
				AnyKeyUpEventBinding(Event.key.keysym.sym);
				Result |= true;
			}
	 		auto It = KeyUpEventBindings.find(Event.key.keysym.sym);
	 		if (It != KeyUpEventBindings.end() && It->second)
	 		{
	 			It->second();
				Result |= true;
	 		}
			Result |= false;
			break;
	 	}
	}

	return Result;
}

bool KeyEventHandler::IsAppropriateEvent(const SDL_Event& Event) const 
{
	return Event.type >= 0x300 && Event.type < 0x400;
}

