#pragma once


#include "CoreMinimal.h"

#define PI_NUMBER	3.14159265358979323846f
					

namespace Math
{
	FORCEINLINE	float Cos(const float Value)
	{
		return cosf(Value);
	}
	FORCEINLINE	float Sin(const float Value)
	{
		return sinf(Value);
	}
	FORCEINLINE	float Tan(const float Value)
	{
		return tanf(Value);
	}
	FORCEINLINE	float Acos(const float Value)
	{
		return acosf(Value);
	}
	FORCEINLINE	float Asin(const float Value)
	{
		return asinf(Value);
	}
	FORCEINLINE	float Atan(const float Value)
	{
		return atanf(Value);
	}

	FORCEINLINE	float ToRadians(const float Degrees)
	{
		return Degrees * PI_NUMBER / 180.f;
	}
	FORCEINLINE float ToDegrees(const float Radians)
	{
		return Radians * 180.f / PI_NUMBER;
	}

	template<typename T>
	FORCEINLINE	T Abs(const T Value)
	{
		return Value >= (T)0 ? Value : -Value;
	}
	template<typename T>
	FORCEINLINE	T Clamp(const T& Min, const T& Max, const T& Value)
	{
		return Value < Min ? Min : (Value > Max ? Max : Value);
	}
	template<typename T>
	FORCEINLINE	T Min(const T& A, const T& B)
	{
		return A < B ? A : B;
	}
	template<typename T>
	FORCEINLINE	T Max(const T& A, const T& B)
	{
		return A > B ? A : B;
	}
	template<typename T>
	FORCEINLINE	T Lerp(const T& A, const T& B, const float Value)
	{
		return A * (1.f - Value) + Value * B;
	}

	FORCEINLINE float SquareRoot(const float Value)
	{
		return sqrtf(Value);
	}

	FORCEINLINE float Square(const float Value)
	{
		return Value * Value;
	}
}