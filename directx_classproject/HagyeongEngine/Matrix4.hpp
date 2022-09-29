/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include "MathLibrary.hpp"

template <typename T>
class Matrix4 {
public:
	
	union {
		T Elements[4 * 4];
		Vector4<T> Column[4];
	};
	
	Matrix4();
	Matrix4(T p_Diagonal);
	
	Vector4<T> GetColumn(int p_Index)
	{
		p_Index * 4;
		return Vector4<T>(Elements[p_Index], Elements[p_Index + 1], Elements[p_Index + 2], Elements[p_Index + 3]);
	}
	
	
	Vector3<T> Multiply(const Vector3<T>& rhs) const;
	Vector4<T> Multiply(const Vector4<T>& rhs) const;
	Matrix4<T>& Multiply(const Matrix4<T>& rhs);
	
	Matrix4<T>& operator*=(const Matrix4<T>& rhs);
	
	static Matrix4<T> Identity();	
	static Matrix4<T> Translate(const Vector3<T>& rhs);	
	static Matrix4<T> Scale(const Vector3<T>& rhs);
	static Matrix4<T> transpose(const Matrix4<T> rhs);
	static Matrix4<T> rotation(T rhs);
	
};

template <typename T>
Vector3<T> operator*(const Matrix4<T>& lhs, const Vector3<T>& rhs);

template <typename T>
Vector4<T> operator*(const Matrix4<T>& lhs, const Vector4<T>& rhs);

template <typename T>
Matrix4<T>  operator*(Matrix4<T> lhs, const Matrix4<T>& rhs);


template <typename T>
Matrix4<T>::Matrix4()
{
	for (int i = 0; i < 4 * 4; i++) 
	{
		Elements[i] = 0.0f;
	}
}
template <typename T>
Matrix4<T>::Matrix4(T rhs)
{
	for (int i = 0; i < 4 * 4; i++) 
	{
		Elements[i] = 0.0f;
	}
	Elements[0 + 0 * 4] = rhs;
	Elements[1 + 1 * 4] = rhs;
	Elements[2 + 2 * 4] = rhs;
	Elements[3 + 3 * 4] = rhs;
}

template <typename T>
Vector3<T> Matrix4<T>::Multiply(const Vector3<T>& rhs) const
{
	return Vector3<T>(
		Column[0].x * rhs.x + Column[1].x * rhs.y + Column[2].x * rhs.z + Column[3].x,
		Column[0].y * rhs.x + Column[1].y * rhs.y + Column[2].y * rhs.z + Column[3].y,
		Column[0].z * rhs.x + Column[1].z * rhs.y + Column[2].z * rhs.z + Column[3].z
		);
}
template <typename T>
Vector4<T> Matrix4<T>::Multiply(const Vector4<T>& rhs) const
{
	return Vector4<T>(
		Column[0].x * rhs.x + Column[1].x * rhs.y + Column[2].x * rhs.z + Column[3].x * rhs.w,
		Column[0].y * rhs.x + Column[1].y * rhs.y + Column[2].y * rhs.z + Column[3].y * rhs.w,
		Column[0].z * rhs.x + Column[1].z * rhs.y + Column[2].z * rhs.z + Column[3].z * rhs.w,
		Column[0].w * rhs.x + Column[1].w * rhs.y + Column[2].w * rhs.z + Column[3].w * rhs.w
		);
}
template <typename T>
Matrix4<T>& Matrix4<T>::Multiply(const Matrix4<T>& rhs)
{
	T Matrix4Data[16];
	
	for (int rows = 0; rows < 4; rows++) 
	{
		for (int columns = 0; columns < 4; columns++) 
		{
			T Sum = 0.0f;
			
			for (int Component = 0; Component < 4; Component++) 
			{
				Sum += Elements[columns + Component * 4] * rhs.Elements[Component + rows * 4];
			}
			
			Matrix4Data[columns + rows * 4] = Sum;
		}
	}
	memcpy(Elements, Matrix4Data, 16 * sizeof(float));
	return *this;
}

template <typename T>
Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& rhs)
{
	return Multiply(rhs);
}

template <typename T>
Vector3<T> operator*(const Matrix4<T>& lhs, const Vector3<T>& rhs)
{
	return lhs.Multiply(rhs);
}
template <typename T>
Vector4<T> operator*(const Matrix4<T>& lhs, const Vector4<T>& rhs)
{
	return lhs.Multiply(rhs);
}
template <typename T>
Matrix4<T> operator*(Matrix4<T> lhs, const Matrix4<T>& rhs)
{
	return lhs.Multiply(rhs);
}

template <typename T>
Matrix4<T> Matrix4<T>::Identity()
{
	return Matrix4<T>(1.0f);
}

template <typename T>
Matrix4<T> Matrix4<T>::Translate(const Vector3<T>& rhs)
{
	Matrix4<T> Result(1.0f);

	Result.Elements[12] = rhs.x;
	Result.Elements[13] = rhs.y;
	Result.Elements[14] = rhs.z;
	return Result;
}

template <typename T>
Matrix4<T> Matrix4<T>::Scale(const Vector3<T>& rhs)
{
	Matrix4<T> Result(1.0f);

	Result.Elements[0] = rhs.x;
	Result.Elements[5] = rhs.y;
	Result.Elements[10] = rhs.z;
	return Result;
}

template <typename T>
Matrix4<T> Matrix4<T>::transpose(const Matrix4<T> rhs)
{
	Matrix4<T> transpose_matrix = rhs;

	transpose_matrix.Column[0].x = rhs.Column[0].x, transpose_matrix.Column[1].x = rhs.Column[0].y, transpose_matrix.Column[2].x = rhs.Column[0].z, transpose_matrix.Column[3].x = rhs.Column[0].w;
	transpose_matrix.Column[0].y = rhs.Column[1].x, transpose_matrix.Column[1].y = rhs.Column[1].y, transpose_matrix.Column[2].y = rhs.Column[1].z, transpose_matrix.Column[3].y = rhs.Column[1].w;
	transpose_matrix.Column[0].z = rhs.Column[2].x, transpose_matrix.Column[1].z = rhs.Column[2].y, transpose_matrix.Column[2].z = rhs.Column[2].z, transpose_matrix.Column[3].z = rhs.Column[2].w;
	transpose_matrix.Column[0].w = rhs.Column[3].x, transpose_matrix.Column[1].w = rhs.Column[3].y, transpose_matrix.Column[2].w = rhs.Column[3].z, transpose_matrix.Column[3].w = rhs.Column[3].w;

	return transpose_matrix;
}

template <typename T>
Matrix4<T> Matrix4<T>::rotation(T rhs)
{
	Matrix4<T> rotation_matrix;

	rotation_matrix.Column[0].x = std::cos(rhs), rotation_matrix.Column[1].x = -std::sin(rhs), rotation_matrix.Column[2].x = 0.0f, rotation_matrix.Column[3].x = 0.0f;
	rotation_matrix.Column[0].y = std::sin(rhs), rotation_matrix.Column[1].y = std::cos(rhs), rotation_matrix.Column[2].y = 0.0f, rotation_matrix.Column[3].y = 0.0f;
	rotation_matrix.Column[0].z = 0.0f, rotation_matrix.Column[1].z = 0.0f, rotation_matrix.Column[2].z = 0.0f, rotation_matrix.Column[3].z = 0.0f;
	rotation_matrix.Column[0].w = 0.0f, rotation_matrix.Column[1].w = 0.0f, rotation_matrix.Column[2].w = 0.0f, rotation_matrix.Column[3].w = 1.0f;

	return rotation_matrix;
}