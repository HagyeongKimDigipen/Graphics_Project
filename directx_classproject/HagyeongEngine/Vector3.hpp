/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include <iostream>
#include "MathFunctions.hpp"

template <typename T>
class Vector3
{
public:
	T width = 600;
	T height = 600;

	T x;
	T y;
	T z;

	Vector3<T>();
	Vector3<T>(const T& valuseX, const T& valueY, const T& valueZ);
	
	T magnitude(Vector3<T> a) noexcept;
	Vector3<T> normalize(Vector3<T> a) noexcept;
	T dot(Vector3<T> a, Vector3<T> b) noexcept;
	T magnitude_squared(Vector3<T> a) noexcept;
	

	Vector3<T>& Add(const Vector3<T>& rhs);
	Vector3<T>& Subtract(const Vector3<T>& rhs);
	Vector3<T>& Multiply(const Vector3<T>& rhs);
	Vector3<T>& Divide(const Vector3<T>& rhs);

	Vector3<T>& operator+=(const Vector3<T>& rhs);
	Vector3<T>& operator-=(const Vector3<T>& rhs);
	Vector3<T>& operator*=(const Vector3<T>& rhs);
	Vector3<T>& operator/=(const Vector3<T>& rhs);

	bool operator==(const Vector3<T>& rhs);
	bool operator!=(const Vector3<T>& rhs);
	
	friend Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs);
	friend Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs);
	friend Vector3<T> operator*(Vector3<T> lhs, const Vector3<T>& rhs);
	friend Vector3<T> operator/(Vector3<T> lhs, const Vector3<T>& rhs);

};


template <typename T>
Vector3<T>::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
template <typename T>
Vector3<T>::Vector3(const T& valuseX, const T& valuseY, const T& valuseZ)
{
	this->x = valuseX;
	this->y = valuseY;
	this->z = valuseZ;
}

template <typename T>
Vector3<T>& Vector3<T>::Add(const Vector3<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::Subtract(const Vector3<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::Multiply(const Vector3<T>& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::Divide(const Vector3<T>& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& rhs)
{
	return Add(rhs);
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& rhs)
{
	return Subtract(rhs);
}

template <typename T>
Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& rhs)
{
	return Multiply(rhs);
}

template <typename T>
Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& rhs)
{
	return Divide(rhs);
}

template <typename T>
bool Vector3<T>::operator==(const Vector3<T>& rhs)
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

template <typename T>
bool Vector3<T>::operator!=(const Vector3<T>& rhs)
{
	return !(*this == rhs);
}


template <typename T>
Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs)
{
	return lhs.Add(rhs);
}

template <typename T>
Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs)
{
	return lhs.Subtract(rhs);
}

template <typename T>
Vector3<T> operator*(Vector3<T> lhs, const Vector3<T>& rhs)
{
	return lhs.Multiply(rhs);
}

template <typename T>
Vector3<T> operator/(Vector3<T> lhs, const Vector3<T>& rhs)
{
	return lhs.Divide(rhs);
}

template <typename T>
T Vector3<T>::magnitude_squared(Vector3<T> a) noexcept
{
	T mag_squared;

	mag_squared = (a.x) * (a.x) + (a.y) * (a.y) + (a.z) * (a.z);

	return mag_squared;
}
template <typename T>
[[nodiscard]] T Vector3<T>::magnitude(const Vector3<T> a) noexcept
{
	T mag;

	mag = std::sqrt(magnitude_squared(a));

	return mag;
}

template <typename T>
Vector3<T> Vector3<T>::normalize(const Vector3<T> a) noexcept
{
	Vector3<T> nor;
	assert(magnitude(a) != 0.0f);
	{
		nor.x = a.x / magnitude(a);
		nor.y = a.y / magnitude(a);
		nor.z = a.z / magnitude(a);
	}

	return nor;
}

template <typename T>
T Vector3<T>::dot(Vector3<T> a, Vector3<T> b) noexcept
{
	T dot_value;

	dot_value = a.x * b.x + a.y * b.y + a.z * b.z;

	return dot_value;
}