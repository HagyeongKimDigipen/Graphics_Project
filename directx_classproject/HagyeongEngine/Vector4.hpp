/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include <iostream>


template <typename T>
class Vector4
{
	public:
		T x;
		T y;
		T z;
		T w;
		
		Vector4<T>();
		Vector4<T>(const T& valueX, const T& valueY, const T& valueZ, const T& valueW);

		Vector4<T> &Add(const Vector4<T> &rhs);
		Vector4<T> &Subtract(const Vector4<T> &rhs);
		Vector4<T> &Multiply(const Vector4<T> & rhs);
		Vector4<T> &Divide(const Vector4<T> & rhs);

		Vector4<T> &operator+=(const Vector4<T> & rhs);
		Vector4<T> &operator-=(const Vector4<T> & rhs);
		Vector4<T> &operator*=(const Vector4<T> & rhs);
		Vector4<T> &operator/=(const Vector4<T> & rhs);

		bool operator==(const Vector4<T> & rhs);
		bool operator!=(const Vector4<T> & rhs);
		
		friend Vector4<T> operator+(Vector4<T> lhs, const Vector4<T> & rhs);
		friend Vector4<T> operator-(Vector4<T> lhs, const Vector4<T> & rhs);
		friend Vector4<T> operator*(Vector4<T> lhs, const Vector4<T> & rhs);
		friend Vector4<T> operator/(Vector4<T> lhs, const Vector4<T> & rhs);
};
	template <typename T>
	Vector4<T>::Vector4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	template <typename T>
	Vector4<T>::Vector4(const T& valueX, const T& valueY, const T& valueZ, const T& valueW)
	{
		this->x = valueX;
		this->y = valueY;
		this->z = valueZ;
		this->w = valueW;
	}

	template <typename T>
	Vector4<T>& Vector4<T>::Add(const Vector4<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	template <typename T>
	Vector4<T>& Vector4<T>::Subtract(const Vector4<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	template <typename T>
	Vector4<T>& Vector4<T>::Multiply(const Vector4<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	template <typename T>
	Vector4<T>& Vector4<T>::Divide(const Vector4<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& rhs)
	{
		return Add(rhs);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& rhs)
	{
		return Subtract(rhs);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator*=(const Vector4<T>& rhs)
	{
		return Multiply(rhs);
	}

	template <typename T>
	Vector4<T>& Vector4<T>::operator/=(const Vector4<T>& rhs)
	{
		return Divide(rhs);
	}

	template <typename T>
	bool Vector4<T>::operator==(const Vector4<T>& rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	template <typename T>
	bool Vector4<T>::operator!=(const Vector4<T>& rhs)
	{
		return !(*this == rhs);
	}

	template <typename T>
	Vector4<T> operator+(Vector4<T> lhs, const Vector4<T>& rhs)
	{
		return lhs.Add(rhs);
	}

	template <typename T>
	Vector4<T> operator-(Vector4<T> lhs, const Vector4<T>& rhs)
	{
		return lhs.Subtract(rhs);
	}

	template <typename T>
	Vector4<T> operator*(Vector4<T> lhs, const Vector4<T>& rhs)
	{
		return lhs.Multiply(rhs);
	}
	
	template <typename T>
	Vector4<T> operator/(Vector4<T> lhs, const Vector4<T>& rhs)
	{
		return lhs.Divide(rhs);
	}
