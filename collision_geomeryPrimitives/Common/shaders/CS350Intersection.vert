/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS350Intersection.vert
Purpose: display ntersection vertex shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 03/05/2022
End Header --------------------------------------------------------*/

#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	mat4 veiwMatrix = projection * camera;
	
	gl_Position = veiwMatrix * transform * vec4(vPosition, 1.0f);
}