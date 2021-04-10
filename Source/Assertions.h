#pragma once
#include "SDL.h"

#ifdef RE_DEBUG
#define LOG(Format, ...)					SDL_Log((Format), __VA_ARGS__)
#define CHECKF(Condition,Format, ...)		if(!(Condition)) { LOG((Format), __VA_ARGS__); __debugbreak(); }
#define CHECK(Condition)					if(!(Condition)) { __debugbreak(); }
#else
#define LOG(Format, ...) 
#define CHECKF(Condition,Format, ...)	
#define CHECK(Condition)	
#endif

