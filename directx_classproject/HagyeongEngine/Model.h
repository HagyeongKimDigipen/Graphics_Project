/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize_rect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Initialize_ell(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Initialize_tri(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Initialize_line(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Initialize_quad(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);

	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const Matrix4<float>& viewProjectionMatrix);

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
	void UpdateWorldMatrix();

	ID3D11Device* devices = nullptr;
	ID3D11DeviceContext* deviceContexts = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexshaders = nullptr;
	ID3D11ShaderResourceView* textureModel = nullptr;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	Vector4<float> posVector; //vector4
	Vector4<float> rotVector; //vector4

	Vector3<float> positions;
	float rotations = 0.0f;
	Vector3<float> scales = { 0.5f,0.5f,0.0f };
	
	Matrix4<float> worldMatrix = Matrix4<float>::Identity();
	Matrix4<float> projectionMatrix = Matrix4<float>::Identity();
	
};
