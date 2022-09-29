/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: load texture
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/20/2021
End Header --------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <malloc.h>
#include <string.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <fstream>
#include "stb_image.h"
#include "Mesh.h"
#include "Scene.h"


class Texture
{
public:
	Texture();
	~Texture();

	GLint LoadTexture(const char* filepath, GLuint shaderID);

	GLint DrawSkyCub(std::vector<std::string> faces);

	unsigned int textureID;

	int widthFBO;
	int heightFBO;
	int nrChannelsFBO;
};

