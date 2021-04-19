#pragma once

#include "MathUtlity.h"

template<typename T>
struct Vector3
{
	//TODO: c++17 required(
	//static_assert(is_same<T, double>::value || is_same<T, float>::value || is_same<T, int>::value);
	T X, Y, Z;
public:

	static float GetAngle(const Vector3<T>& L, const Vector3<T>& R, bool bIsInRadians)
	{
		float ResultRad = Math::Acos(L.Dot(R) / (L.GetLength() * R.GetLength()));
		return (bIsInRadians ? ResultRad : Math::ToDegrees(ResultRad));
	}

	Vector3(T InX, T InY, T InZ) :
		X(InX), Y(InY), Z(InZ)
	{}

	Vector3(const Vector3& InVector) :
		X(InVector.X), Y(InVector.Y), Z(InVector.Z)
	{}

	Vector3(T UniformValue) :
		X(UniformValue), Y(UniformValue), Z(UniformValue)
	{}

	Vector3()
	{}

	Vector3 operator*(T Value) const
	{
		return Vector3(X * Value, Y * Value, Z * Value);
	}

	Vector3& operator*=(T Value)
	{
		X *= Value;
		Y *= Value;
		Z *= Value;
		return *this;
	}

	Vector3 operator+(T Value) const
	{
		return Vector3(X + Value, Y + Value, Z + Value);
	}

	Vector3& operator+=(T Value)
	{
		X += Value;
		Y += Value;
		Z += Value;
		return *this;
	}

	Vector3 operator-(T Value) const
	{
		return Vector3(X - Value, Y - Value, Z - Value);
	}

	Vector3 operator-() const
	{
		return Vector3(-X, -Y, -Z);
	}

	Vector3& operator-=(T Value)
	{
		X -= Value;
		Y -= Value;
		Z -= Value;
		return *this;
	}

	Vector3 operator/(T Value) const
	{
		return Vector3(X / Value, Y / Value, Z / Value);
	}

	Vector3& operator/=(T Value)
	{
		X /= Value;
		Y /= Value;
		Z /= Value;
		return *this;
	}

	float Dot(const Vector3& Value) const
	{
		return X * Value.X + Y * Value.Y + Z * Value.Z;
	}

	Vector3 Cross(const Vector3& Value) const
	{
		return Vector3((Y * Value.Z) - (Z * Value.Y), (Z * Value.X) - (X * Value.Z), (X * Value.Y) - (Y * Value.X));
	}

	Vector3 operator+(const Vector3& InVector) const
	{
		return Vector3(X + InVector.X, Y + InVector.Y, Z + InVector.Z);
	}

	Vector3& operator+=(const Vector3& InVector)
	{
		X += InVector.X;
		Y += InVector.Y;
		Z += InVector.Z;
		return *this;
	}

	Vector3 operator-(const Vector3& InVector) const
	{
		return Vector3(X - InVector.X, Y - InVector.Y, Z - InVector.Z);
	}

	Vector3& operator-=(const Vector3& InVector)
	{
		X -= InVector.X;
		Y -= InVector.Y;
		Z -= InVector.Z;
		return *this;
	}

	bool operator==(const Vector3 InVector)
	{
		return X == InVector.X && Y == InVector.Y && Z == InVector.Z;
	}

	float GetLength() const
	{
		return Math::SquareRoot(GetSquaredLength());
	}

	float GetSquaredLength() const
	{
		return Math::Square(X) + Math::Square(Y) + Math::Square(Z);
	}

	Vector3& Normalize()
	{
		float Length = GetLength();
		X /= Length;
		Y /= Length;
		Z /= Length;
		return *this;
	}

	Vector3 GetNormalized() const
	{
		float Length = GetLength();
		return Vector3(X / Length, Y / Length, Z / Length);
	}
};

#ifdef RE_DEBUG
#include <iostream>
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& dt)
{
    os << dt.X  << ' ' << dt.Y << ' ' << dt.Z;
    return os;
}
#endif //RE_DEBUG

typedef Vector3<double> Vector3d;
typedef Vector3<float>	Vector3f;
typedef Vector3<int>	Vector3i;
