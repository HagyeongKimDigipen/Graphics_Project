/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AABB.h
Purpose: header of AABB.cpp 
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

//point aabb sphere plane triangle ray

class AABB
{
public:
	AABB() {};
	AABB(const glm::vec3& minAABB, const glm::vec3& maxAABB);
	~AABB() 
	{
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &norm);
	}
	void SetUp();
	
	glm::vec3 min;
	glm::vec3 max;

/////////////
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<GLuint> indices;

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint norm = 0;
};
