/*
*	hakyung.kim
*	Assigment 1
*	CS250
*	spring 2020
*/

#include "Affine.h"

Affine::Affine(void)
{
	row[0].x = 0;
	row[0].y = 0;
	row[0].z = 0;
	row[0].w = 0;

	row[1].x = 0;
	row[1].y = 0;
	row[1].z = 0;
	row[1].w = 0;

	row[2].x = 0;
	row[2].y = 0;
	row[2].z = 0;
	row[2].w = 0;

	row[3].x = 0;
	row[3].y = 0;
	row[3].z = 0;
	row[3].w = 1;
}

Hcoord::Hcoord(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

Point::Point(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
	w = 1;
}

Vector::Vector(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
	w = 0;
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	row[0].x = Lx.x;
	row[0].y = Ly.x;
	row[0].z = Lz.x;
	row[0].w = D.x;

	row[1].x = Lx.y;
	row[1].y = Ly.y;
	row[1].z = Lz.y;
	row[1].w = D.y;

	row[2].x = Lx.z;
	row[2].y = Ly.z;
	row[2].z = Lz.z;
	row[2].w = D.z;

	row[3].x = Lx.w;
	row[3].y = Ly.w;
	row[3].z = Lz.w;
	row[3].w = D.w;
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	Hcoord M;

	M.x = u.x + v.x;
	M.y = u.y + v.y;
	M.z = u.z + v.z;
	M.w = u.w + v.w;

	return M;
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	Hcoord M;

	M.x = u.x - v.x;
	M.y = u.y - v.y;
	M.z = u.z - v.z;
	M.w = u.w - v.w;

	return M;
}

Hcoord operator-(const Hcoord& v)
{
	Hcoord M;

	M.x = -v.x;
	M.y = -v.y;
	M.z = -v.z;
	M.w = -v.w;

	return M;
}

Hcoord operator*(float r, const Hcoord& v)
{
	Hcoord M;

	M.x = v.x * r;
	M.y = v.y * r;
	M.z = v.z * r;
	M.w = v.w * r;

	return M;
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	Hcoord M;

	M.x = v.x * A.row[0].x + v.y * A.row[0].y + v.z * A.row[0].z + v.w * A.row[0].w;
	M.y = v.x * A.row[1].x + v.y * A.row[1].y + v.z * A.row[1].z + v.w * A.row[1].w;
	M.z = v.x * A.row[2].x + v.y * A.row[2].y + v.z * A.row[2].z + v.w * A.row[2].w;
	M.w = v.x * A.row[3].x + v.y * A.row[3].y + v.z * A.row[3].z + v.w * A.row[3].w;
	
	return M;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix M;

	for (int column = 0; column < 4; column++)
	{
		M.row[0][column] = A.row[0].x * B.row[0][column] + A.row[0].y * B.row[1][column] + A.row[0].z * B.row[2][column] + A.row[0].w * B.row[3][column];
		M.row[1][column] = A.row[1].x * B.row[0][column] + A.row[1].y * B.row[1][column] + A.row[1].z * B.row[2][column] + A.row[1].w * B.row[3][column];
		M.row[2][column] = A.row[2].x * B.row[0][column] + A.row[2].y * B.row[1][column] + A.row[2].z * B.row[2][column] + A.row[2].w * B.row[3][column];
		M.row[3][column] = A.row[3].x * B.row[0][column] + A.row[3].y * B.row[1][column] + A.row[3].z * B.row[2][column] + A.row[3].w * B.row[3][column];
	}

	return M;
}

float dot(const Vector& u, const Vector& v)
{
	float dot_value;
	dot_value = (u.x * v.x) + (u.y * v.y) + (u.z * v.z) + (u.w * v.w);
	return dot_value;
}

float abs(const Vector& v)
{
	float abs_value;
	abs_value = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return abs_value;
}

Vector normalize(const Vector& v)
{
	Vector normal_vector;
	normal_vector.x = v.x / abs(v);
	normal_vector.y = v.y / abs(v);
	normal_vector.z = v.z / abs(v);
	normal_vector.w = v.w / abs(v);

	return normal_vector;
}

Vector cross(const Vector& u, const Vector& v)
{
	Vector cross;
	
	cross.x = u.y * v.z - u.z * v.y;
	cross.y = u.z * v.x - u.x * v.z;
	cross.z = u.x * v.y - u.y * v.x;

	return cross;
}

Affine rotate(float t, const Vector& v)
{
	// cos t * I + ((1 - cos t)/abs(v)^2) * vv^t + (sin t/ abs(v)) * Au
	
	Affine rotation;

	float cos_ = 1.f - cos(t);
	float abs_v = abs(v) * abs(v);
	 
	rotation.row[0].x = ((v.x * v.x * cos_) / abs_v) + cos(t);
	rotation.row[0].y = ((v.x * v.y * cos_) / abs_v) + ((-v.z * sin(t)) / abs(v));
	rotation.row[0].z = ((v.x * v.z * cos_) / abs_v) + ((v.y * sin(t)) / abs(v));
	rotation.row[0].w = 0;

	rotation.row[1].x = ((v.y * v.x * cos_) / abs_v) + ((v.z * sin(t)) / abs(v));
	rotation.row[1].y = ((v.y * v.y * cos_) / abs_v) + cos(t);
	rotation.row[1].z = ((v.y * v.z * cos_) / abs_v) + ((-v.x * sin(t)) / abs(v));
	rotation.row[1].w = 0;

	rotation.row[2].x = ((v.z * v.x * cos_) / abs_v) + ((-v.y * sin(t)) / abs(v));
	rotation.row[2].y = ((v.z * v.y * cos_) / abs_v) + ((v.x * sin(t)) / abs(v));
	rotation.row[2].z = ((v.z * v.z * cos_) / abs_v) + cos(t);
	rotation.row[2].w = 0;

	return rotation;
}

Affine translate(const Vector& v)
{
	Affine translation;
	
	translation.row[0].x = 1;
	translation.row[0].y = 0;
	translation.row[0].z = 0;
	translation.row[0].w = v.x;

	translation.row[1].x = 0;
	translation.row[1].y = 1;
	translation.row[1].z = 0;
	translation.row[1].w = v.y;

	translation.row[2].x = 0;
	translation.row[2].y = 0;
	translation.row[2].z = 1;
	translation.row[2].w = v.z;

	return translation;
}

Affine scale(float r)
{
	Affine scale_r;

	scale_r.row[0].x = r;
	scale_r.row[0].y = 0;
	scale_r.row[0].z = 0;
	scale_r.row[0].w = 0;

	scale_r.row[1].x = 0;
	scale_r.row[1].y = r;
	scale_r.row[1].z = 0;
	scale_r.row[1].w = 0;

	scale_r.row[2].x = 0;
	scale_r.row[2].y = 0;
	scale_r.row[2].z = r;
	scale_r.row[2].w = 0;

	return scale_r;
}

Affine scale(float rx, float ry, float rz)
{
	Affine scale;

	scale.row[0].x = rx;
	scale.row[0].y = 0;
	scale.row[0].z = 0;
	scale.row[0].w = 0;

	scale.row[1].x = 0;
	scale.row[1].y = ry;
	scale.row[1].z = 0;
	scale.row[1].w = 0;

	scale.row[2].x = 0;
	scale.row[2].y = 0;
	scale.row[2].z = rz;
	scale.row[2].w = 0;

	return scale;
}

Affine inverse(const Affine& A)
{
	Affine det; // det and change the sign {+ - +}{- + -}{+ - +}
	Affine det_t; // transpose the det and multiply determinant
	Affine negative_v;

	float determinant;

	negative_v.row[0].x = 1;
	negative_v.row[0].y = 0;
	negative_v.row[0].z = 0;
	negative_v.row[0].w = -A.row[0].w;

	negative_v.row[1].x = 0;
	negative_v.row[1].y = 1;
	negative_v.row[1].z = 0;
	negative_v.row[1].w = -A.row[1].w;

	negative_v.row[2].x = 0;
	negative_v.row[2].y = 0;
	negative_v.row[2].z = 1;
	negative_v.row[2].w = -A.row[2].w;

	det.row[0].x = A.row[1].y * A.row[2].z - A.row[2].y * A.row[1].z;
	det.row[0].y = -(A.row[1].x * A.row[2].z - A.row[2].x * A.row[1].z);
	det.row[0].z = A.row[1].x * A.row[2].y - A.row[2].x * A.row[1].y;
	det.row[0].w = 0;
	
	det.row[1].x = -(A.row[0].y * A.row[2].z - A.row[2].y * A.row[0].z);
	det.row[1].y = A.row[0].x * A.row[2].z - A.row[2].x * A.row[0].z;
	det.row[1].z = -(A.row[0].x * A.row[2].y - A.row[2].x * A.row[0].y);
	det.row[1].w = 0;
	
	det.row[2].x = A.row[0].y * A.row[1].z - A.row[1].y * A.row[0].z;
	det.row[2].y = -(A.row[0].x * A.row[1].z - A.row[1].x * A.row[0].z);
	det.row[2].z = A.row[0].x * A.row[1].y - A.row[1].x * A.row[0].y;
	det.row[2].w = 0;

	determinant = (1 / (A.row[0].x * det.row[0].x + A.row[0].y * det.row[0].y + A.row[0].z * det.row[0].z));

	det_t.row[0].x = det.row[0].x * determinant;
	det_t.row[0].y = det.row[1].x * determinant;
	det_t.row[0].z = det.row[2].x * determinant;
	det_t.row[0].w = 0;
					 
	det_t.row[1].x = det.row[0].y * determinant;
	det_t.row[1].y = det.row[1].y * determinant;
	det_t.row[1].z = det.row[2].y * determinant;
	det_t.row[1].w = 0;

	det_t.row[2].x = det.row[0].z * determinant;
	det_t.row[2].y = det.row[1].z * determinant;
	det_t.row[2].z = det.row[2].z * determinant;
	det_t.row[2].w = 0;

	det = det_t * negative_v;

	return det;
}


