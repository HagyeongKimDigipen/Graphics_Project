/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300SKYShader.frag
Purpose: fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/25/2021
End Header --------------------------------------------------------*/
#version 410 core

out vec3 FragColor;

in vec3 TexCoords;

uniform samplerCube skyBoxs;

void main()
{
    FragColor = texture(skyBoxs, TexCoords.xyz).xyz;
} 