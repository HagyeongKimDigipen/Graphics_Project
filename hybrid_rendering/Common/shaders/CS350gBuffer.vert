/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS350gBuufer.vert
Purpose:gbuffer fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 02/01/2022
End Header --------------------------------------------------------*/

#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 normal;
out vec2 tex;

uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
    vec4 worldPos = transform * vec4(vPosition, 1.f);

    fragPos = worldPos.xyz;

    normal = normalize(mat3(transpose(inverse(transform))) * vNormal);

    tex = aTexCoords;

    mat4 veiwMatrix = projection * camera;

    gl_Position = veiwMatrix * worldPos;
}