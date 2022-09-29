#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: load texture
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/20/2021
End Header --------------------------------------------------------*/

#include "Texture.h"
#include <iostream>
#include <sstream>
#include <fstream>


Texture::Texture()
{
	widthFBO = 1024;
	heightFBO = 800;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

GLint Texture::LoadTexture(const char* filepath, GLuint /*shaderID*/)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int nrChannels;

	GLenum glType = GL_RGB;
	
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 1)
		{
			glType = GL_RED;
		}

		if (nrChannels == 4)
		{
			glType = GL_RGBA;
		}

		stbi_set_flip_vertically_on_load(1);

		glTexImage2D(GL_TEXTURE_2D, 0, glType, width, height, 0, glType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}

GLint Texture::ReadPPMFile(std::filesystem::path const& filepath)
{
	unsigned int textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::ifstream file(filepath);

	std::stringstream fileStream;
	fileStream << file.rdbuf();

	int line = 0;
	int index = 0;

	int width = 0;
	int height = 0;
	int res = 0;

	unsigned char* data = nullptr;

	while (!fileStream.eof())
	{
		while (line < 2)
		{
			std::string files;

			std::getline(fileStream, files);

			if (files[0] == '#')
			{
				continue;
			}

			if (line == 0)
			{
				if (!(files == "P3" || files == "P6"))
				{
					throw std::runtime_error("file error");
				}

				line++;
			}

			else if (line == 1)
			{
				std::stringstream stream(files);
				stream >> width >> height;
				data = new unsigned char[width * height * 3 + 1];
				line++;
			}
		}

		fileStream >> res;
		data[index++] = static_cast<unsigned char>(res);
	}

	file.close();

	GLenum glType = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, glType, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLint Texture::DrawSkyCub(std::vector<std::string> faces)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	GLenum glType = GL_RGB;

	int width, height, nrChannels;
	unsigned char* data;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);

		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			if (nrChannelsFBO == 1)
			{
				glType = GL_RED;
			}

			if (nrChannelsFBO == 4)
			{
				glType = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glType, width, height, 0, glType, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

	return textureID;
}
