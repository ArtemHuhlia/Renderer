#pragma once
#include "Event/KeyEventHandler.h"
#include "Event/MouseEventHandler.h"
#include "Event/ApplicationEventHandler.h"

struct EventManager
{
	KeyEventHandler			KeybordEvents;
	MouseEventHandler		MouseEvents;
	ApplicationEventHandler ApplicationEvents;
public:
	void PollEvents();
};
