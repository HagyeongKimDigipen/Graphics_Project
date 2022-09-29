/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS350normal.vert
Purpose: display normal fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 09/15/2021
End Header --------------------------------------------------------*/

#version 410 core
layout (location = 0) in vec3 vPosition;

uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	mat4 veiwMatrix = projection * camera;
	
	gl_Position = veiwMatrix * transform * vec4(vPosition, 1.0f);
}