#pragma once

#include "MathUtlity.h"
#include "Math/Vector3.h"

template<typename T>
struct Vector4
{
	//TODO: c++17 required(
	//static_assert(is_same<T, double>::value || is_same<T, float>::value || is_same<T, int>::value);
	T X, Y, Z, W;
public:

	static float GetAngle(const Vector4<T>& L, const Vector4<T>& R, bool bIsInRadians)
	{
		float ResultRad = Math::Acos(L.Dot(R) / (L.GetLength() * R.GetLength()));
		return (bIsInRadians ? ResultRad : Math::ToDegrees(a));
	}

	Vector4(T InX, T InY, T InZ, T InW) :
		X(InX), Y(InY), Z(InZ), W(InW)
	{}

	Vector4(const Vector4& InVector) :
		X(InVector.X), Y(InVector.Y), Z(InVector.Z), W(InVector.W)
	{}

	Vector4(const Vector3<T>& InVector, float WComp) :
		X(InVector.X), Y(InVector.Y), Z(InVector.Z), W(WComp)
	{}

	Vector4(T UniformValue = 0.f) :
		X(UniformValue), Y(UniformValue), Z(UniformValue), W(UniformValue)
	{}

	Vector4 operator*(T Value) const
	{
		return Vector4(X * Value, Y * Value, Z * Value, W * Value,);
	}

	Vector4& operator*=(T Value)
	{
		X *= Value;
		Y *= Value;
		Z *= Value;
		W *= Value;
		return *this;
	}

	Vector4 operator+(T Value) const
	{
		return Vector4(X + Value, Y + Value, Z + Value, W + Value);
	}

	Vector4& operator+=(T Value)
	{
		X += Value;
		Y += Value;
		Z += Value;
		W += Value;
		return *this;
	}

	Vector4 operator-(T Value) const
	{
		return Vector4(X - Value, Y - Value, Z - Value,  W - Value);
	}

	Vector4& operator-=(T Value)
	{
		X -= Value;
		Y -= Value;
		Z -= Value;
		W -= Value;
		return *this;
	}

	Vector4 operator/(T Value) const
	{
		return Vector4(X / Value, Y / Value, Z / Value, W / Value);
	}

	Vector4& operator/=(T Value)
	{
		X /= Value;
		Y /= Value;
		Z /= Value;
		W /= Value;
		return *this;
	}

	float Dot(const Vector4& Value) const
	{
		return X * Value.X + Y * Value.Y + Z * Value.Z + W * Value.W;
	}

	Vector4 operator+(const Vector4& InVector) const
	{
		return Vector4(X + InVector.X, Y + InVector.Y, Z + InVector.Z, W + InVector.W);
	}

	Vector4& operator+=(const Vector4& InVector)
	{
		X += InVector.X;
		Y += InVector.Y;
		Z += InVector.Z;
		W += InVector.W;
		return *this;
	}

	Vector4 operator-(const Vector4& InVector) const
	{
		return Vector4(X - InVector.X, Y - InVector.Y, Z - InVector.Z, W - InVector.W);
	}

	Vector4& operator-=(const Vector4& InVector)
	{
		X -= InVector.X;
		Y -= InVector.Y;
		Z -= InVector.Z;
		W -= InVector.W;
		return *this;
	}

	bool operator==(const Vector4 InVector)
	{
		return X == InVector.X && Y == InVector.Y && Z == InVector.Z && W == InVector.W;
	}

	float GetLength() const
	{
		return Math::SquareRoot(GetSquaredLength());
	}

	float GetSquaredLength() const
	{
		return Math::Square(X) + Math::Square(Y) + Math::Square(Z) + Math::Square(W);
	}

	Vector4& Normalize()
	{
		float Length = GetLength();
		X /= Length;
		Y /= Length;
		Z /= Length;
		W /= Length;
		return *this;
	}

	Vector4 GetNormalized() const
	{
		float Length = GetLength();
		return Vector4(X / Length, Y / Length, Z / Length, W / Length);
	}
};

#ifdef RE_DEBUG
#include <iostream>
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector4<T>& dt)
{
    os << dt.X  << ' ' << dt.Y << ' ' << dt.Z << ' ' << dt.W;
    return os;
}
#endif //RE_DEBUG


typedef Vector4<double> Vector4d;
typedef Vector4<float>	Vector4f;
typedef Vector4<int>	Vector4i;
