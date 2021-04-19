#include "Event/MouseEventHandler.h"
#include "CoreMinimal.h"


bool MouseEventHandler::BindOnMouseMotionEvent(MouseMotionEventFunction Function)
{
	CHECK(Function);

	bool bWasBounded = false;
	if (MouseMotionEventBinding)
		bWasBounded = true;

	MouseMotionEventBinding = Function;

	return bWasBounded;
}

bool MouseEventHandler::BindOnMouseWheelEvent(MouseWheelEventFunction Function)
{
	CHECK(Function);

	bool bWasBounded = false;
	if (MouseWheelEventBinding)
		bWasBounded = true;

	MouseWheelEventBinding = Function;

	return bWasBounded;
}

bool MouseEventHandler::BindOnMouseButtonDownEvent(ButtonCode Code, MouseButtonDownEventFunction Function)
{
	CHECK(Function);

	return MouseButtonDownEventBindings.insert({ Code, Function }).second;
}

bool MouseEventHandler::BindOnMouseButtonUpEvent(ButtonCode Code, MouseButtonUpEventFunction Function)
{
	CHECK(Function);

	return MouseButtonUpEventBindings.insert({ Code, Function }).second;
}

bool MouseEventHandler::HandleEvent(const union SDL_Event& Event) const
{
	switch (Event.type)
	{
		case SDL_MOUSEMOTION:
		{
			if (MouseMotionEventBinding)
			{
				MouseMotionEventBinding(Event.motion.x, Event.motion.y, Event.motion.xrel, Event.motion.yrel);
				return true;
			}
			return false;
		}
		case SDL_MOUSEWHEEL:
		{
			if (MouseWheelEventBinding)
			{
				MouseWheelEventBinding(Event.wheel.x, Event.wheel.y, Event.wheel.direction);
				return true;
			}
			return false;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			auto It = MouseButtonDownEventBindings.find(Event.button.button);
			if (It != MouseButtonDownEventBindings.end() && It->second)
			{
				It->second(Event.button.x, Event.button.y, Event.button.clicks);
				return true;
			}
			return false;
		}
		case SDL_MOUSEBUTTONUP:
		{
			auto It = MouseButtonUpEventBindings.find(Event.button.button);
			if (It != MouseButtonUpEventBindings.end() && It->second)
			{
				It->second(Event.button.x, Event.button.y, Event.button.clicks);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool MouseEventHandler::IsAppropriateEvent(const SDL_Event& Event) const
{
	return Event.type >= 0x400 && Event.type < 0x600;
}
