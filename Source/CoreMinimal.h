#pragma once

#include "Assertions.h"

#define INFOLOG_SIZE 512

#ifdef RE_DEBUG

#if defined( _MSC_VER)
#define NOINLINE  __declspec(noinline)
#else
#error UNSUPPORTED COMPILER!
#endif //RE_NOINLINE

#else RE_RELEASE 
#define RE_NOINLINE 
#endif //RE_DEBUG

#if defined( _MSC_VER)
#define FORCEINLINE __forceinline
#else
#error UNSUPPORTED COMPILER!
#endif //RE_NOINLINE

#define BIT(x) 1<<(x);
