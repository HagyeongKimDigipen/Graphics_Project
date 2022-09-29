/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "Model.h"

constexpr float PI = 3.1415926535897932384626433832795f;
constexpr float HALF_PI = PI / 2.0f;
constexpr float QUARTER_PI = PI / 4.0f;
constexpr float TWO_PI = 2.0f * PI;

bool Model::Initialize_rect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->devices = device;
	this->deviceContexts = deviceContext;
	this->textureModel = texture;
	this->cb_vs_vertexshaders = &cb_vs_vertexshader;

	//Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, 0.0f, 0.0f,0.0f,0.0f,0.0f, 1.0f), //Bottom Left
		Vertex(-0.5f,   0.5f, 0.0f,0.0f,0.0f,0.0f ,0.0f, 0.0f), //Top Left
		Vertex(0.5f,   0.5f, 0.0f, 0.0f,0.0f,0.0f,1.0f, 0.0f), //Top Right
		Vertex(0.5f, -0.5f, 0.0f, 0.0f,0.0f,0.0f,1.0f, 1.0f), //Bottom Right	
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->devices, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		0,1,2,0,2,3
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->devices, indices, ARRAYSIZE(indices));
	
	this->UpdateWorldMatrix();
	return true;
}

bool Model::Initialize_ell(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture,
	ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->devices = device;
	this->deviceContexts = deviceContext;
	this->textureModel = texture;
	this->cb_vs_vertexshaders = &cb_vs_vertexshader;

	Vertex v[120];

	size_t Range;

	float Angle = TWO_PI / 100;

	for (Range = 0; Range < 120; Range++)
	{
		v[Range] = Vertex(0.3f * sin(Angle * Range)*2, 0.3f * cos(Angle * Range), -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->devices, v, ARRAYSIZE(v));

	DWORD indices[360];
	
	int check = 1;
	
	for (int i = 0; i < 360; i++)
	{
		if (i % 3 == 0)
		{
			indices[i] = 0;
		}
		else if (i % 3 == 1)
		{
			indices[i] = check;
			check++;
		}
		else
		{
			indices[i] = check;
		}
	}

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->devices, indices, ARRAYSIZE(indices));

	this->UpdateWorldMatrix();
	return true;
}

bool Model::Initialize_tri(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture,
	ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->devices = device;
	this->deviceContexts = deviceContext;
	this->textureModel = texture;
	this->cb_vs_vertexshaders = &cb_vs_vertexshader;

	//Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, -0.5f,0.0f,0.0f,0.0f, 0.0f, 1.0f),
		Vertex(0.0f,   0.5f, -0.5f, 0.0f,0.0f,0.0f,0.0f, 0.0f),
		Vertex(0.5f,   -0.5f, -0.5f,0.0f,0.0f,0.0f, 1.0f, 0.0f),
	};

	//Load Vertex Data

	HRESULT hr = this->vertexBuffer.Initialize(this->devices, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		0,1,2,
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->devices, indices, ARRAYSIZE(indices));

	this->UpdateWorldMatrix();
	return true;
}

bool Model::Initialize_line(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture,
	ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->devices = device;
	this->deviceContexts = deviceContext;
	this->textureModel = texture;
	this->cb_vs_vertexshaders = &cb_vs_vertexshader;

	//Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, -0.5f,0.0f,0.0f,0.0f, 0.0f, 0.0f),
		Vertex(0.5f,   -0.5f, -0.5f, 0.0f,0.0f,0.0f,0.0f, 0.0f),
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->devices, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		0,1,
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->devices, indices, ARRAYSIZE(indices));

	this->UpdateWorldMatrix();
	return true;
}

bool Model::Initialize_quad(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture,
	ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->devices = device;
	this->deviceContexts = deviceContext;
	this->textureModel = texture;
	this->cb_vs_vertexshaders = &cb_vs_vertexshader;

	//Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, 0.0f, 0.0f,0.0f,0.0f,0.0f, 1.0f), 
		Vertex(-0.3f,   0.5f, 0.0f,0.0f,0.0f,0.0f ,0.0f, 0.0f), 
		Vertex(0.5f,   0.5f, 0.0f, 0.0f,0.0f,0.0f,1.0f, 0.0f),
		Vertex(0.3f, -0.5f, 0.0f, 0.0f,0.0f,0.0f,1.0f, 1.0f), 
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->devices, v, ARRAYSIZE(v));

	DWORD indices[] =
	{
		0,1,2,0,2,3
	};

	//Load Index Data
	hr = this->indexBuffer.Initialize(this->devices, indices, ARRAYSIZE(indices));

	this->UpdateWorldMatrix();
	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->textureModel = texture;
}

void Model::Draw(const Matrix4<float>& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cb_vs_vertexshaders->data.mat = this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cb_vs_vertexshaders->data.mat = Matrix4<float>::transpose(this->cb_vs_vertexshaders->data.mat);
	this->cb_vs_vertexshaders->ApplyChanges();
	this->deviceContexts->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshaders->GetAddressOf());

	this->deviceContexts->PSSetShaderResources(0, 1, &this->textureModel); //Set Texture
	this->deviceContexts->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	UINT offset = 0;
	this->deviceContexts->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContexts->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0); //Draw
}

void Model::UpdateWorldMatrix()
{
	this->worldMatrix = Matrix4<float>::Identity();
	
	Matrix4<float> translateion;
	Matrix4<float> rotation;
	Matrix4<float> scaling;

	translateion = Matrix4<float>::Translate(positions);
	rotation = Matrix4<float>::rotation(rotations);
	scaling = Matrix4<float>::Scale(scales);

	this->worldMatrix = translateion * rotation * scaling;
}

const Vector4<float>& Model::GetPositionVector() const
{
	return this->posVector;
}

const Vector3<float>& Model::GetPositionFloat3() const
{
	return this->positions;
}

const Vector4<float>& Model::GetRotationVector() const
{
	return this->rotVector;
}

void Model::SetPosition(const Vector4<float>& pos)
{
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
	this->positions = Vector3<float>(x, y, z);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const Vector4<float>& pos)
{
	this->posVector += pos;
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
	this->positions.x += x;
	this->positions.y += y;
	this->positions.z += z;
	this->UpdateWorldMatrix();
}

void Model::SetRotation(const Vector4<float>& rot)
{
	this->rotVector = rot;
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const Vector4<float>& rot)
{
	this->rotVector += rot;
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(float x)
{
	rotations += x;
	this->UpdateWorldMatrix();
}

void Model::AdjustScale(const Vector4<float>& scaleValue)
{
	this->scales.x *= scaleValue.x;
	this->scales.y *= scaleValue.y;

	this->UpdateWorldMatrix();
}

void Model::AdjustScale(const Vector3<float>& scaleValue)
{
	this->scales.x /= scaleValue.x;
	this->scales.y /= scaleValue.y;

	this->UpdateWorldMatrix();
}

