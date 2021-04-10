#pragma once

#include "Event/EventHandler.h"

class MouseEventHandler : public EventHandler
{
	using ButtonCode = uint8_t; //see SDL defindes

	using MouseMotionEventFunction = std::function<void(int32_t X, int32_t Y, int32_t RelX, int32_t RelY)>;
	using MouseWheelEventFunction = std::function<void(int32_t X, int32_t Y, uint32_t Direction /* = SDL_MOUSEWHEEL_NORMAL or SDL_MOUSEWHEEL_FLIPPED*/)>;
	using MouseButtonDownEventFunction = std::function<void(int32_t X, int32_t Y, uint8_t Clicks /* = 1 or 2*/)>;
	using MouseButtonUpEventFunction = std::function<void(int32_t X, int32_t Y, uint8_t Clicks /* = 1 or 2*/)>;

	MouseMotionEventFunction MouseMotionEventBinding;
	MouseWheelEventFunction MouseWheelEventBinding;
	std::unordered_map<ButtonCode,MouseButtonDownEventFunction> MouseButtonDownEventBindings;
	std::unordered_map<ButtonCode,MouseButtonUpEventFunction> MouseButtonUpEventBindings;	
public:
	bool BindOnMouseMotionEvent(MouseMotionEventFunction Function);
	bool BindOnMouseWheelEvent(MouseWheelEventFunction Function);
	bool BindOnMouseButtonDownEvent(ButtonCode Code, MouseButtonDownEventFunction Function);
	bool BindOnMouseButtonUpEvent(ButtonCode Code, MouseButtonUpEventFunction Function);

	virtual bool HandleEvent(const union SDL_Event& Event) const override;
	virtual bool IsAppropriateEvent(const SDL_Event& Event) const override;
};
