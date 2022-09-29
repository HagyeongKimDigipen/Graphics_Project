/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include <DirectXMath.h>
#include "MathLibrary.hpp"
struct CB_VS_vertexshader
{
	Matrix4<float> mat;
};

//8 -> 16bytes

struct CB_PS_pixelshader
{
	float alpha = 1.0f;
};