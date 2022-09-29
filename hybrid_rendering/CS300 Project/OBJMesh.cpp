/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OBJMesh.cpp
Purpose: Load OBJ file 
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 02/01/2021
End Header --------------------------------------------------------*/
#include "OBJMesh.h"

OBJMesh::OBJMesh(std::string path)
{
	paths = path;

	objReader->OBJReader::ReadOBJFile(path, &myMesh, OBJReader::ReadMethod::LINE_BY_LINE, false);
	myMesh = *(objReader->getMesh());
}

OBJMesh::~OBJMesh()
{
	cleanUp();
}

void OBJMesh::objLoad(const char* vertex, const char* fragment)
{
	programID = myShader.LoadShaders(vertex, fragment);
	setupBuffer();
}

void OBJMesh::setupBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &norm);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &uv);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 *sizeof(GLfloat), myMesh.getVertexBuffer(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(myMesh.getVertexBufferSize()) * 3 * sizeof(GLfloat),myMesh.getVertexNormals(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh.getIndexBufferSize() * sizeof(GLuint),myMesh.getIndexBuffer(), GL_STATIC_DRAW);

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

void OBJMesh::cleanUp()
{
	glDeleteProgram(programID);
}

void OBJMesh::readSection()
{
	std::stringstream ss;

	for (const auto& file : std::filesystem::directory_iterator("models/"))
	{
		std::ifstream in_file(file.path().string());

		std::string line = "";

		while (std::getline(in_file, line))
		{
			std::ifstream in_file("models/" + line);

			if (in_file)
			{
				power.push_back(new OBJMesh("models/" + line));

			}
		}
	}
}
