/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DrawScene.cpp
Purpose: draw mesh and load obj file
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/15/2021
End Header --------------------------------------------------------*/
#include "DrawScene.h"
#include <glm/gtc/epsilon.hpp>
#include <iostream>

DrawScene::DrawScene(int /*windowWidth*/, int /*windowHeight*/, DrawScene::mesh type, std::string path, int types, bool entity)
{
	VertexArrayID = 0;
	meshType = type;
	paths = path;
	angleOfRotation = 0.0f;
	vbo = 0;
	ebo = 0;
	norm = 0;
	uv = 0;
	NormalVao = 0;
	NormalfaceVao = 0;
	vertexCheck = false;
	faceCheck = false;
	uvTypes = types;
	isEntity = entity;
}

DrawScene::~DrawScene()
{
	CleanUp();
}

int DrawScene::Init()
{
	float start = 0.f;
	float stack = 20.f;
	float sector = 20.f;
	const float PI = 3.141592f;

	if (meshType == DrawScene::mesh::Sphere || meshType == DrawScene::mesh::lamp)
	{
		const float radius = 0.05f;

		float x, y, z;

		float sectorAngle, stackAngle;

		glm::vec3 nvertices(0.1f, 0.1f, 0.1f);

		for (float i = start; i <= stack; ++i)
		{
			stackAngle = PI / 2 - i * (PI / stack);
			z = radius * sinf(stackAngle);

			for (float j = 0; j <= sector; ++j)
			{
				sectorAngle = j * (2 * PI / sector);
				x = radius * cosf(stackAngle) * cosf(sectorAngle);
				y = radius * cosf(stackAngle) * sinf(sectorAngle);

				myMesh.vertexBuffer.push_back({ x,y,z });

				nvertices.x = x * 1.0f;
				nvertices.y = y * 1.0f;
				nvertices.z = z * 1.0f;

				myMesh.vertexNormals.push_back(nvertices);
			}
		}

		GLint first, second;

		for (float i = 0; i < stack; ++i)
		{
			first = static_cast<GLint>(i * (sector + 1));
			second = static_cast<GLint>(first + sector + 1);

			for (int j = 0; j < sector; ++j, ++first, ++second)
			{
				if (i != 0)
				{
					myMesh.vertexIndices.push_back(first);
					myMesh.vertexIndices.push_back(second);
					myMesh.vertexIndices.push_back(first + 1);
				}

				if (i != (stack - 1.f))
				{
					myMesh.vertexIndices.push_back(first + 1);
					myMesh.vertexIndices.push_back(second);
					myMesh.vertexIndices.push_back(second + 1);
				}
			}
		}
	}
	else if (meshType == DrawScene::mesh::Obj)
	{
		objReader->OBJReader::ReadOBJFile(paths, &myMesh, OBJReader::ReadMethod::LINE_BY_LINE, false, uvTypes, isEntity);
		myMesh = *(objReader->getMesh());
	}
	else if (meshType == DrawScene::mesh::ObjN)
	{
		objReader->OBJReader::ReadOBJFile(paths, &myMesh, OBJReader::ReadMethod::BLOCK_IO, false);
		myMesh = *(objReader->getMesh());
	}
	else if (meshType == DrawScene::mesh::Orbit)
	{
		float radius = 3.0f;

		glm::vec3 first = glm::vec3(0.1f);
		glm::vec3 last = glm::vec3(0.1f);

		for (float angle = 0; angle <= 2 * PI; angle += PI / stack)
		{
			first = { radius * cos(angle) / 2, 0, radius * sin(angle) / 2 };
			last = { radius * cos(angle + PI / stack) / 2, 0, radius * sin(angle + PI / stack) / 2 };

			myMesh.vertexBuffer.push_back(first);
			myMesh.vertexBuffer.push_back(last);
		}
	}
	else if (meshType == DrawScene::mesh::floor)
	{
		objReader->OBJReader::ReadOBJFile("../Common/models/floor.obj", &myMesh, OBJReader::ReadMethod::LINE_BY_LINE, false);
		myMesh = *(objReader->getMesh());
	}

	DoNormal();
	matrixUpdate();
	SetupBuffers();
	SetUpNormalBuffer();
	SetUpNormalFaceBuffer();

	return Scene::Init();
}

void DrawScene::CleanUp()
{
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &norm);
	glDeleteBuffers(1, &uv);
	glDeleteBuffers(1, &NormalVao);
	glDeleteBuffers(1, &NormalfaceVao);

	delete objReader;
}

int DrawScene::Render()
{
	matrixUpdate();

	if (meshType != DrawScene::mesh::lamp)
	{
		glBindVertexArray(VertexArrayID);
	}
	else
	{
		glBindVertexArray(lightVAO);
	}

	glm::vec3 sphereCentroid = myMesh.getModelCentroid();

	if (meshType != mesh::Orbit)
	{
		glDrawElements(GL_TRIANGLES, GLsizei(myMesh.getIndexBufferSize()), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glLineWidth(50);
		glDrawArrays(GL_LINES, 0, myMesh.getVertexBufferSize());
	}

	if (vertexCheck == true)
	{
		glBindVertexArray(NormalVao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(myMesh.vertexNormalDisplay.size() * 2));
		glBindVertexArray(0);

	}
	if (faceCheck == true)
	{
		glBindVertexArray(NormalfaceVao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(myMesh.vertexNormalDisplay.size() * 2));
		glBindVertexArray(0);

	}

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	return 0;
}

int DrawScene::postRender()
{
	if (meshType == DrawScene::mesh::Sphere || meshType == DrawScene::mesh::Orbit || meshType == DrawScene::mesh::lamp)
	{
		angleOfRotation += 0.001f;
	}

	return 0;
}

void DrawScene::SetupBuffers()
{
	if (meshType != DrawScene::mesh::lamp && meshType != DrawScene::mesh::Obj)
	{
		glGenVertexArrays(1, &VertexArrayID);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &norm);
		glGenBuffers(1, &ebo);

		glBindVertexArray(VertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexBuffer(), GL_STATIC_DRAW);//

		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexNormals(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.getIndexBufferSize() * sizeof(GLuint), myMesh.getIndexBuffer(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glBindVertexArray(0);
	}
	else if (meshType == DrawScene::mesh::Obj)
	{
		glGenVertexArrays(1, &VertexArrayID);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &norm);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &uv);

		glBindVertexArray(VertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexBuffer(), GL_STATIC_DRAW);//

		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexNormals(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.getIndexBufferSize() * sizeof(GLuint), myMesh.getIndexBuffer(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, uv);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 2 * sizeof(GLfloat), myMesh.getVertexUVs(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uv);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

		glBindVertexArray(0);
	}
	else
	{
		glGenVertexArrays(1, &lightVAO);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &norm);
		glGenBuffers(1, &ebo);

		glBindVertexArray(lightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexBuffer(), GL_STATIC_DRAW);//

		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat), myMesh.getVertexNormals(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.getIndexBufferSize() * sizeof(GLuint), myMesh.getIndexBuffer(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, norm);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

}

void DrawScene::matrixUpdate()
{
	modelNormal = glm::translate(centerModel) * glm::scale(glm::vec3(modelSize));
	modelMat = glm::rotate(angleOfRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(scaleVector) * glm::translate(center) * modelNormal;
}

void DrawScene::DoNormal()
{
	int totalSize = static_cast<int>(myMesh.vertexBuffer.size());

	//widnow size 1024 * 800

	float maxX = 514;
	float maxY = 400;
	float maxZ = FLT_MAX;

	float minX = -514;
	float minY = -400;
	float minZ = FLT_MIN;

	for (int i = 0; totalSize != 0; i++)
	{
		if (myMesh.vertexBuffer[i].x < maxX)
		{
			maxX = myMesh.vertexBuffer[i].x;
		}
		if (myMesh.vertexBuffer[i].x > minX)
		{
			minX = myMesh.vertexBuffer[i].x;
		}

		if (myMesh.vertexBuffer[i].y < maxY)
		{
			maxY = myMesh.vertexBuffer[i].y;
		}
		if (myMesh.vertexBuffer[i].y > minY)
		{
			minY = myMesh.vertexBuffer[i].y;
		}

		if (myMesh.vertexBuffer[i].z < maxZ)
		{
			maxZ = myMesh.vertexBuffer[i].z;
		}
		if (myMesh.vertexBuffer[i].z > minZ)
		{
			minZ = myMesh.vertexBuffer[i].z;
		}

		totalSize--;
	}

	glm::vec3 total = glm::vec3(maxX, maxY, maxZ) + glm::vec3(minX, minY, minZ);

	float max = std::max((minY - maxY), (minZ - maxZ));
	max = std::max(max, (minX - maxX));

	modelSize = abs(2.f / max);
	centerModel = -(total / 2.f * modelSize);
}

void DrawScene::SetUpNormalBuffer()
{
	glGenVertexArrays(1, &NormalVao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(NormalVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, myMesh.vertexNormalDisplay.size() * 3 * sizeof(GLfloat), myMesh.vertexNormalDisplay.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.vertexIndices.size() * sizeof(GLfloat), myMesh.vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}

void DrawScene::SetUpNormalFaceBuffer()
{
	glGenVertexArrays(1, &NormalfaceVao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(NormalfaceVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, myMesh.faceNormalsDisplay.size() * 3 * sizeof(GLfloat), myMesh.faceNormalsDisplay.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.vertexIndices.size() * sizeof(GLfloat), myMesh.vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}
