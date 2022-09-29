/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AABB.cpp
Purpose: AABB primitives
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/

#include "AABB.h"

AABB::AABB(const glm::vec3& minAABB, const glm::vec3& maxAABB)
{
	vertex.clear();

	min = minAABB;
	max = maxAABB;

	max.x = std::max(min.x, max.x);
	max.y = std::max(min.y, max.y);
	max.z = std::max(min.z, max.z);

	min.x = std::min(min.x, max.x);
	min.y = std::min(min.y, max.y);
	min.z = std::min(min.z, max.z);

	glm::vec3 pointTop[8] = { glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z),
		glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z) };

	glm::vec3 pointBottom[8] = { glm::vec3(min.x, min.y, min.z) , glm::vec3(max.x, min.y, min.z) ,glm::vec3(max.x, min.y, min.z),  glm::vec3(max.x, min.y, max.z) ,
		glm::vec3(max.x, min.y, max.z) ,glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z) };

	glm::vec3 pointLeft[8] = { glm::vec3(min.x, max.y, max.z),glm::vec3(min.x, max.y, min.z),glm::vec3(min.x, max.y, min.z),glm::vec3(min.x, min.y, min.z),
		glm::vec3(min.x, min.y, min.z),glm::vec3(min.x, min.y, max.z),glm::vec3(min.x, min.y, max.z),glm::vec3(min.x, max.y, max.z) };

	glm::vec3 pointRight[8] = { glm::vec3(max.x, max.y, max.z),glm::vec3(max.x, max.y, min.z),glm::vec3(max.x, max.y, min.z),glm::vec3(max.x, min.y, min.z),glm::vec3(max.x, min.y, min.z),
		glm::vec3(max.x, min.y, max.z),glm::vec3(max.x, min.y, max.z),glm::vec3(max.x, max.y, max.z) };

	for (int i = 0; i < 8; i++)
	{
		vertex.push_back(pointTop[i]);
	}

	for (int i = 0; i < 8; i++)
	{
		vertex.push_back(pointBottom[i]);
	}

	for (int i = 0; i < 8; i++)
	{
		vertex.push_back(pointLeft[i]);
	}

	for (int i = 0; i < 8; i++)
	{
		vertex.push_back(pointRight[i]);
	}
}


void AABB::SetUp()
{
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &norm);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);

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
