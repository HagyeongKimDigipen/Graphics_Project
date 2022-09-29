/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Plane.h
Purpose: header of Plane.cpp 
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>

//point aabb sphere plane triangle ray

class Plane
{
public:
	Plane() { normal = glm::vec3(0); }
	Plane(glm::vec3 normals, float distance);
	Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	~Plane()
	{
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &norm);
	}

	glm::vec3 normal;
	float distance = 0;

	void SetUp();

	glm::mat4 model = glm::mat4(1.f);

/////////////
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normalsV;
	std::vector<GLuint> indices;

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint norm = 0;
};
