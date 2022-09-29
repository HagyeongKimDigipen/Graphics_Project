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
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <set>
#include <glm/gtc/epsilon.hpp>

class Mesh
{
public:
	GLfloat* getVertexBuffer();             // attribute 0
	GLfloat* getVertexNormals();            // attribute 1
	GLfloat* getVertexUVs();                // attribute 2

	GLfloat* getVertexNormalsForDisplay();  // attribute 0

	unsigned int getVertexBufferSize();
	unsigned int getVertexCount();
	unsigned int getVertexNormalCount();

	GLuint* getIndexBuffer();
	unsigned int getIndexBufferSize();
	unsigned int getTriangleCount();

	glm::vec3   getModelScale();
	glm::vec3   getModelCentroid();
	glm::vec3   getCentroidVector(glm::vec3 vVertex);


	GLfloat& getNormalLength();
	void setNormalLength(GLfloat nLength);

	void initData();

	int calcVertexNormals(GLboolean bFlipNormals = false);

	void calcVertexNormalsForDisplay(GLboolean bFlipNormals = false);

	enum class UVType {
		PLANAR_UV = 0,
		CYLINDRICAL_UV,
		SPHERICAL_UV,
		CUBE_MAPPED_UV
	};

	int         calcUVs(bool Entity, Mesh::UVType uvType = UVType::PLANAR_UV);
	glm::vec2   calcCubeMap(glm::vec3 vEntity);

//private:
	std::vector<glm::vec3>    vertexBuffer;
	std::vector<GLuint>       vertexIndices;
	std::vector<glm::vec2>    vertexUVs;
	std::vector<glm::vec3>    vertexNormals;
	std::vector<glm::vec3>    vertexNormalDisplay;
	std::vector<glm::vec3>    faceNormalsDisplay;

	glm::vec3               boundingBox[2];
	GLfloat                 normalLength = 0.f;
};
