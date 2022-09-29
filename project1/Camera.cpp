/*
*	hakyung.kim
*	Assigment 3
*	CS250
*	spring 2020
*/

#include "Camera.h"

Camera::Camera(void) : eye(0, 0, 0), right(1, 0, 0), up(0, 1, 0), back(0, 0, -1),
near(0.1f), far(10.f), width(0.f), height(0.f), distance(0.f)
{
}

Camera::Camera(const Point& E, const Vector& look, const Vector& vp,
	float fov, float aspect, float near, float far)
{
	eye = E;

	right = normalize(cross(look, vp));
	back = Vector{ -(look.x / abs(look)), -(look.y / abs(look)), -(look.z / abs(look)) };
	up = cross(back, right);

	width = width = 2.f * 15.f * tan(fov / 2.f);
	height = width / aspect;
	distance = width / (2.f * tan(fov / 2.f));
	
	this->near = near;
	this->far = far;
}

Point Camera::Eye(void) const
{
	return this->eye;
}

Vector Camera::Right(void) const
{
	return this->right;
}

Vector Camera::Up(void) const
{
	return this->up;
}

Vector Camera::Back(void) const
{
	return normalize(back);
}

Vector Camera::ViewportGeometry(void) const
{
	Vector view;

	view.x = width;
	view.y = height;
	view.z = distance;

	return view;
}

float Camera::NearDistance(void) const
{
	return this->near;
}

float Camera::FarDistance(void) const
{
	return this->far;
}

Camera& Camera::Zoom(float factor)
{
	this->width *= factor;
	this->height *= factor;

	return *this;
}

Camera& Camera::Forward(float distance_increment)
{
	this->eye = this->eye - distance_increment * this->back;

	return *this;
}


Camera& Camera::Yaw(float angle)
{
	this->right = rotate(angle, this->up) * this-> right;
	this->back = rotate(angle, this->up) * this->back;

	return *this;
}

Camera& Camera::Pitch(float angle)
{
	this->up = rotate(angle, this->right) * this->up;
	this->back = rotate(angle, this->right) * this->back;

	return *this;

}

Camera& Camera::Roll(float angle)
{
	this->right = rotate(angle, this->back) * this->right;
	this->up = rotate(angle, this->back) * this->up;

	return *this;

}
