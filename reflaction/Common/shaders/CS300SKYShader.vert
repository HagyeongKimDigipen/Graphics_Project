/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300SKYShader.vert
Purpose: vertex shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/25/2021
End Header --------------------------------------------------------*/
#version 410 core

layout (location = 0) in vec3 vPosition;

uniform mat4 camera;
uniform mat4 projection;
uniform mat4 transform;

out vec3 TexCoords;

void main()
{
    vec3 vs_position = vPosition - 1.0;
    TexCoords = vPosition - 1.0;
    gl_Position = projection * camera * transform * vec4(vs_position, 1.0);
}  
