/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OBJMesh.h
Purpose: header of OBJMesh.cpp
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 02/01/2021
End Header --------------------------------------------------------*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "OBJReader.h"

#include "Mesh.h"
#include "shader.hpp"

class OBJMesh
{
public:
	OBJMesh(std::string path);
	~OBJMesh();

	void objLoad(const char* vertex, const char* fragment);
	void setupBuffer();
	void cleanUp();
	void readSection();

	Mesh getMesh() { return myMesh; }

	Mesh myMesh;
	std::vector<OBJMesh*> power;

	OBJReader* objReader = new(OBJReader);

	GLuint  vao;

	GLuint  vbo;
	GLuint  norm;
	GLuint  ebo;
	GLuint  ubo;
	GLuint  uv;

	GLuint programID;
	Shader myShader;

	std::string paths;
};




