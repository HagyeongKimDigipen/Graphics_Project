/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include <DirectXMath.h>


struct Vertex
{
	Vertex() : pos(0.0f,0.0f,0.0f), color(0.0f,0.0f,0.0f),texCoord(0.0f,0.0f){}
	Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
		: pos(x, y, z), color(r, g, b), texCoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 texCoord;

};