/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LampShader.vert
Purpose: for lamp that use only diffuse
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/20/2021
End Header --------------------------------------------------------*/

#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 camera;
uniform mat4 projection;
uniform mat4 transform;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	Normal = mat3(transpose(inverse(transform)))* aNormal;
	gl_Position = projection * camera * transform * vec4(vPosition, 1.0f);
	FragPos = vec3(transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0));
}