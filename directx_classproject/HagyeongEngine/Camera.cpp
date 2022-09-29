/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "Camera.h"

Camera::Camera()
{
	this->positions = Vector3<float>(0.0f, 0.0f, 0.0f);
	this->rotations = 0.0f;
	this->scales = { 1.0f,1.0f,1.0f };
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float /*vDegrees*/, float/*spectRatio*/, float /*nearZ*/, float /*farZ*/)
{
	//this->projectionMatrix = XMMatrixOrthographicOffCenterLH(0.0f, fovDegrees, aspectRatio, 0.0f, nearZ, farZ);
}

const Matrix4<float>& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const Matrix4<float>& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const Vector4<float>& Camera::GetPositionVector() const
{
	return this->posVector;
}

const Vector3<float>& Camera::GetPositionFloat3() const
{
	return this->positions;
}

const Vector4<float>& Camera::GetRotationVector() const
{
	return this->rotVector;
}

void Camera::SetPosition(const Vector4<float>& pos)
{
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->positions = Vector3<float>(x, y, z);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const Vector4<float>& pos)
{
	this->posVector += pos;
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->positions.x += x;
	this->positions.y += y;
	this->positions.z += z;
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const Vector4<float>& rot)
{
	this->rotVector = rot;
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const Vector4<float>& rot)
{
	this->rotVector += rot;
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x)
{
	rotations += x;
	this->UpdateViewMatrix();
}

void Camera::AdjustScale(const Vector4<float>& scaleValue)
{
	this->scales.x *= scaleValue.x;
	this->scales.y *= scaleValue.y;
	
	this->UpdateViewMatrix();
}

void Camera::AdjustScale(const Vector3<float>& scaleValue)
{
	this->scales.x /= scaleValue.x;
	this->scales.y /= scaleValue.y;

	this->UpdateViewMatrix();
}


void Camera::UpdateViewMatrix()
{
	Matrix4<float> translateion;
	Matrix4<float> rotation;
	Matrix4<float> scaling;

	translateion = Matrix4<float>::Translate(positions);
	rotation = Matrix4<float>::rotation(rotations);
	scaling = Matrix4<float>::Scale(scales);

	this->viewMatrix = scaling * rotation * translateion;
	
}
