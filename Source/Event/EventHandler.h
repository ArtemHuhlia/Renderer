#pragma once

class EventHandler
{
	float DeltaTime = 0.f;
public:
	//returns true if event has been handled 
	virtual bool HandleEvent(const union SDL_Event& Event) const = 0;
	virtual bool IsAppropriateEvent(const SDL_Event& Event) const  = 0;
};
