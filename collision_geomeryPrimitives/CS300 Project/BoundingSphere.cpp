/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingSphere.cpp
Purpose: BoundingSphere primitives
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/

#include "BoundingSphere.h"
float PI = 3.141592F;

BoundingSphere::BoundingSphere(float radius, glm::vec3 centers)
{
	vertex.clear();
	indices.clear();
	normal.clear();

	center = centers;
	r = radius;

	float stack = 20.f;
	float start = 0.f;
	float sector = 20.f;

	float x, y, z;

	float sectorAngle, stackAngle;

	glm::vec3 nvertices(0.1f, 0.1f, 0.1f);

	for (float i = start; i <= stack; ++i)
	{
		stackAngle = PI / 2 - i * (PI / stack);
		z = center.z + radius * sinf(stackAngle);

		for (float j = 0; j <= sector; ++j)
		{
			sectorAngle = j * (2 * PI / sector);
			x = center.x + (radius * cosf(stackAngle) * cosf(sectorAngle));
			y = center.y + radius * cosf(stackAngle) * sinf(sectorAngle);

			vertex.push_back({ x,y,z });

			nvertices.x = x * 1.0f;
			nvertices.y = y * 1.0f;
			nvertices.z = z * 1.0f;

			normal.push_back(nvertices);
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
				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);
			}

			if (i != (stack - 1.f))
			{
				indices.push_back(first + 1);
				indices.push_back(second);
				indices.push_back(second + 1);
			}
		}
	}
}

void BoundingSphere::SetUp()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &SphereNorm);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, SphereNorm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex.size()) * 3 * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, SphereNorm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}
