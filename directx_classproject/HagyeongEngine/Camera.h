/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include <DirectXMath.h>
#include "MathLibrary.hpp"
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const Matrix4<float>& GetViewMatrix() const;
	const Matrix4<float>& GetProjectionMatrix() const;

	const Vector4<float>& GetPositionVector() const;
	const Vector3<float>& GetPositionFloat3() const;
	const Vector4<float>& GetRotationVector() const;

	void SetPosition(const Vector4<float>& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const Vector4<float>& pos);
	void AdjustPosition(float x, float y, float z);
	
	void SetRotation(const Vector4<float>& rot);
	void AdjustRotation(const Vector4<float>& rot);
	void AdjustRotation(float x);

	void AdjustScale(const Vector4<float>& scaleValue);
	void AdjustScale(const Vector3<float>& scaleValue);

private:
	void UpdateViewMatrix();
	
	Vector4<float> posVector; //vector4
	Vector4<float> rotVector; //vector4
	
	Vector3<float> positions;
	float rotations;
	Vector3<float> scales;
	
	Matrix4<float> viewMatrix;
	Matrix4<float> projectionMatrix = Matrix4<float>::Identity();
};