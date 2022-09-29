/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: header of Mesh.cpp (draw shape and calculate normal)
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 09/23/2021
End Header --------------------------------------------------------*/
#include "Mesh.h"

// Initialize the data members in the mesh
void Mesh::initData()
{
	vertexBuffer.clear();
	vertexIndices.clear();
	vertexUVs.clear();
	vertexNormals.clear();
	vertexNormalDisplay.clear();
	normalLength = 0.00f;

	return;
}

GLfloat* Mesh::getVertexBuffer()
{
	return reinterpret_cast<GLfloat*>(vertexBuffer.data());
}

GLfloat* Mesh::getVertexNormals()
{
	return reinterpret_cast<GLfloat*>(vertexNormals.data());
}

GLfloat* Mesh::getVertexUVs()
{
	return reinterpret_cast<GLfloat*>(vertexUVs.data());
}

GLfloat* Mesh::getVertexNormalsForDisplay()
{
	return reinterpret_cast<GLfloat*>(vertexNormalDisplay.data());
}

GLuint* Mesh::getIndexBuffer()
{
	return vertexIndices.data();
}

unsigned int Mesh::getVertexBufferSize()
{
	return (unsigned int)vertexBuffer.size();
}

unsigned int Mesh::getIndexBufferSize()
{
	return (unsigned int)vertexIndices.size();
}

unsigned int Mesh::getTriangleCount()
{
	return getIndexBufferSize() / 3;
}

unsigned int Mesh::getVertexCount()
{
	return getVertexBufferSize();
}

unsigned int Mesh::getVertexNormalCount()
{
	return static_cast<unsigned int>(vertexNormalDisplay.size());
}

glm::vec3  Mesh::getModelScale()
{
	glm::vec3 scale = boundingBox[1] - boundingBox[0];

	if (scale.x == 0.0)
		scale.x = 1.0;

	if (scale.y == 0.0)
		scale.y = 1.0;

	if (scale.z == 0.0)
		scale.z = 1.0;

	return scale;
}


glm::vec3  Mesh::getModelCentroid()
{
	return glm::vec3(boundingBox[0] + boundingBox[1]) * 0.5f;
}

glm::vec3 Mesh::getCentroidVector(glm::vec3 vVertex)
{
	return glm::normalize(vVertex - getModelCentroid());
}

struct compareVec
{
	bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
	{
		glm::highp_bvec3 result = glm::lessThan(lhs, rhs);

		return glm::epsilonEqual(lhs.x, rhs.x, FLT_EPSILON) ?
			(glm::epsilonEqual(lhs.y, rhs.y, FLT_EPSILON) ? (glm::epsilonEqual(lhs.z, rhs.z, FLT_EPSILON) ? false : result.z) : result.y) : result.x;
	}
};

int Mesh::calcVertexNormals(GLboolean bFlipNormals)
{
	int rFlag = -1;

	// vertices and indices must be populated
	if (vertexBuffer.empty() || vertexIndices.empty())
	{
		std::cout << "Cannot calculate vertex normals for empty mesh." << std::endl;
		return rFlag;
	}

	// Initialize vertex normals
	GLuint  numVertices = getVertexCount();
	vertexNormals.resize(numVertices, glm::vec3(0.0f));
	vertexNormalDisplay.resize(static_cast<GLsizeiptr>(numVertices * 2.0), glm::vec3(0.0f));
	faceNormalsDisplay.resize(static_cast<GLsizeiptr>(getTriangleCount() * 2.0), glm::vec3(0.0f));

	std::vector< std::set< glm::vec3, compareVec > >  vNormalSet;
	vNormalSet.resize(numVertices);

	// For every face
	glm::uint index = 0;
	for (; index < vertexIndices.size(); )
	{
		GLuint a = vertexIndices.at(index++);
		GLuint b = vertexIndices.at(index++);
		GLuint c = vertexIndices.at(index++);

		glm::vec3  vA = vertexBuffer[a];
		glm::vec3  vB = vertexBuffer[b];
		glm::vec3  vC = vertexBuffer[c];

		glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);

		center.x = (vA.x + vB.x + vC.x) / 3.f;
		center.y = (vA.y + vB.y + vC.y) / 3.f;
		center.z = (vA.z + vB.z + vC.z) / 3.f;

		// Edge vectors
		glm::vec3  E1 = vB - vA;
		glm::vec3  E2 = vC - vA;

		glm::vec3  E1f = vB - center;
		glm::vec3  E2f = vC - center;

		glm::vec3  N = glm::normalize(glm::cross(E1, E2));
		glm::vec3  Nf = glm::normalize(glm::cross(E1f, E2f));

		if (bFlipNormals)
			N = N * -1.0f;

		if (bFlipNormals)
			Nf = Nf * -1.0f;

		// For vertex a
		vNormalSet.at(a).insert(N);
		vNormalSet.at(b).insert(N);
		vNormalSet.at(c).insert(N);

		setNormalLength(0.05f);

		faceNormalsDisplay[(static_cast<GLsizeiptr>(index / 3.f) - 1) * 2] = center;
		faceNormalsDisplay[(static_cast<GLsizeiptr>(index / 3.f) - 1) * 2 + 1] = center + normalLength * Nf;
	}

	// Now sum up the values per vertex
	for (int i = 0; i < vNormalSet.size(); ++i)
	{
		glm::vec3  vNormal(0.0f);

		setNormalLength(0.05f);

		auto nIt = vNormalSet[i].begin();
		while (nIt != vNormalSet[i].end())
		{
			vNormal += (*nIt);
			++nIt;
		}

		// save vertex normal
		vertexNormals[i] = glm::normalize(vNormal);

		// save normal to display
		glm::vec3  vA = vertexBuffer[i];

		vertexNormalDisplay[static_cast <GLsizeiptr>(2.0 * i)] = vA;
		vertexNormalDisplay[static_cast<GLsizeiptr>(2.0 * i) + 1] = vA + (normalLength * vertexNormals[i]);
	}

	// success
	rFlag = 0;

	return rFlag;
}

void Mesh::calcVertexNormalsForDisplay(GLboolean /*bFlipNormals*/)
{
	GLuint numVertices = getVertexCount();
	vertexNormalDisplay.resize(static_cast<GLsizeiptr>(numVertices * 2.0), glm::vec3(0.0f));

	for (int iNormal = 0; iNormal < vertexNormals.size(); ++iNormal)
	{
		glm::vec3 normal = vertexNormals[iNormal] * normalLength;

		vertexNormalDisplay[static_cast <GLsizeiptr>(2.0 * iNormal)] = vertexBuffer[iNormal];
		vertexNormalDisplay[static_cast<GLsizeiptr>(2.0 * iNormal) + 1] = vertexBuffer[iNormal] + normal;
	}
}

GLfloat& Mesh::getNormalLength()
{
	return normalLength;
}

void Mesh::setNormalLength(GLfloat nLength)
{
	normalLength = nLength;
}

int Mesh::calcUVs(bool Entity, Mesh::UVType uvType)
{
	int rFlag = -1;

	// clear any existing UV
	vertexUVs.clear();

	glm::vec3 delta = getModelScale();

	for (int nVertex = 0; nVertex < vertexBuffer.size(); ++nVertex)
	{
		glm::vec3 V = vertexBuffer[nVertex];
		glm::vec2 uv(0.0f);

		glm::vec3 normVertex = glm::vec3((V.x - boundingBox[0].x) / delta.x,
			(V.y - boundingBox[0].y) / delta.y,
			(V.z - boundingBox[0].z) / delta.z);

		glm::vec3 centroidVec = V;
		if (Entity == true)
		{
			centroidVec = glm::normalize(normVertex);
		}

		float theta(0.0f);
		float z(0.0f);
		float phi(0.0f);

		switch (uvType)
		{
		case UVType::PLANAR_UV:
			uv.x = (normVertex.x - (-1.f)) / (2.f);
			uv.y = (normVertex.z - (-1.f)) / (2.f);
			break;

		case UVType::CYLINDRICAL_UV:
			theta = glm::degrees(static_cast<float>(atan2(centroidVec.z, centroidVec.x)));
			theta += 180.0f;

			z = (centroidVec.y + 1.0f) * 0.5f;

			uv.x = theta / 360.0f;
			//uv.y = z;
			uv.y = (centroidVec.y - boundingBox[0].y) / (boundingBox[1].y - boundingBox[0].y);
			break;

		case UVType::SPHERICAL_UV:
			theta = glm::degrees(static_cast<float>(glm::atan(centroidVec.z, centroidVec.x)));
			theta += 180.0f;

			z = centroidVec.y;
			phi = glm::degrees(glm::acos(z / centroidVec.length()));

			uv.x = theta / 360.0f;
			uv.y = 1.0f - (phi / 180.0f);
			break;

		case UVType::CUBE_MAPPED_UV:
			uv = calcCubeMap(centroidVec);
			break;
		}

		vertexUVs.push_back(uv);
	}

	return rFlag;
}

glm::vec2 Mesh::calcCubeMap(glm::vec3 vEntity)
{
	float x = vEntity.x;
	float y = vEntity.y;
	float z = vEntity.z;

	float absX = abs(x);
	float absY = abs(y);
	float absZ = abs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	float maxAxis = 0.f;
	float uc = 0.f;
	float vc = 0.f;
	glm::vec2 uv = glm::vec2(0.0);

	// POSITIVE X
	if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
	{
		maxAxis = absX;
		uc = -z;
		vc = y;
	}

	// NEGATIVE X
	else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
	{
		maxAxis = absX;
		uc = z;
		vc = y;
	}

	// POSITIVE Y
	else if (bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		maxAxis = absY;
		uc = x;
		vc = -z;
	}

	// NEGATIVE Y
	else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		maxAxis = absY;
		uc = x;
		vc = z;
	}

	// POSITIVE Z
	else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		maxAxis = absZ;
		uc = x;
		vc = y;
	}

	// NEGATIVE Z
	else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		maxAxis = absZ;
		uc = -x;
		vc = y;
	}

	uv.s = 0.5f * (uc / maxAxis + 1.0f);
	uv.t = 0.5f * (vc / maxAxis + 1.0f);

	return uv;
}