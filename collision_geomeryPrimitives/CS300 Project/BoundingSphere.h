/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingSphere.h
Purpose: header of BoundingSphere.cpp
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/
#pragma once

#include "Point3D.h"

//point aabb sphere plane triangle ray

class BoundingSphere
{
public:
	BoundingSphere() {};
	BoundingSphere(float radius, glm::vec3 centers);
	~BoundingSphere() 
	{
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &SphereNorm);
	}

	void SetUp();

	float r;
	glm::vec3 center = glm::vec3{ 0.f };

/////////////
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<GLuint> indices;

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint SphereNorm = 0;
	GLuint ebo = 0;
};
