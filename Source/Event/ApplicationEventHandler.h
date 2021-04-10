#pragma once
#include "Event/EventHandler.h"

class ApplicationEventHandler : public EventHandler
{
	using  QuitEventFunction = std::function<void(void)>;

	QuitEventFunction QuitEventBinding;
public:
	bool BindOnQuitEvent(QuitEventFunction Function);

	virtual bool HandleEvent(const union SDL_Event& Event) const override;
	virtual bool IsAppropriateEvent(const SDL_Event& Event) const override;
};
