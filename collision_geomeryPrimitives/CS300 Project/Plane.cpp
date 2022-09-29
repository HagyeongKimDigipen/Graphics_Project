/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Plane.cpp
Purpose: Plane primitives
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/

#include "Plane.h"

Plane::Plane(glm::vec3 normals, float distances)
{
	normal = glm::normalize(normals);
	distance = distances;

	vertex.clear();
	normalsV.clear();
	indices.clear();

	glm::vec3 right = glm::cross(normal, glm::vec3(0.f, 1.f, 0.f));
	glm::vec3 cross = glm::cross(right, normal);

	glm::mat4 rotation;

	rotation[0][0] = right.x;
	rotation[0][1] = cross.x;
	rotation[0][2] = -normal.x;
	rotation[0][3] = 0.f;

	rotation[1][0] = right.y;
	rotation[1][1] = cross.y;
	rotation[1][2] = -normal.y;
	rotation[1][3] = 0.f;
				   
	rotation[2][0] = right.z;
	rotation[2][1] = cross.z;
	rotation[2][2] = -normal.z;
	rotation[2][3] = 0.f;
				   
	rotation[3][0] = 0.f;
	rotation[3][1] = 0.f;
	rotation[3][2] = 0.f;
	rotation[3][3] = 1.f;

	model = glm::translate(model, normal * distance) * rotation;
	model = glm::scale(model, glm::vec3(1.f));

	vertex.push_back(glm::vec3(-1, -1, 0));
	vertex.push_back(glm::vec3(-1, 1, 0));
	vertex.push_back(glm::vec3(1, 1, 0));
	vertex.push_back(glm::vec3(1, -1, 0));
}

Plane::Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 nor = glm::cross(b - a, c - a);
	Plane(nor, glm::dot(nor, a));
}

void Plane::SetUp()
{
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &norm);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), normalsV.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}
