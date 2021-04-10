#pragma once

#include "SDL.h"
#include "Event/EventHandler.h"

class KeyEventHandler : public EventHandler
{
	using KeyDownEventFunction = std::function<void(void)>;
	using KeyUpEventFunction = std::function<void(void)>;
	using AnyKeyDownEventFunction = std::function<void(SDL_Keycode)>;
	using AnyKeyUpEventFunction = std::function<void(SDL_Keycode)>;

	std::unordered_map<SDL_Keycode, KeyDownEventFunction> KeyDownEventBindings;
	std::unordered_map<SDL_Keycode, KeyUpEventFunction>		KeyUpEventBindings;
	AnyKeyDownEventFunction									AnyKeyDownEventBinding;
	AnyKeyUpEventFunction									AnyKeyUpEventBinding;
public:
	KeyEventHandler() {}

	//returns true if binding has been overrided
	bool BindOnKeyDownEvent(SDL_Keycode KeyCode, KeyDownEventFunction Function);
	bool BindOnKeyUpEvent(SDL_Keycode KeyCode, KeyUpEventFunction Function);
	bool BindOnAnyKeyDownEvent(AnyKeyDownEventFunction Function);
	bool BindOnAnyKeyUpEvent(AnyKeyUpEventFunction Function);

	//Interface realisation
	virtual bool HandleEvent(const SDL_Event& Event) const override;

	virtual bool IsAppropriateEvent(const SDL_Event& Event) const override;
};