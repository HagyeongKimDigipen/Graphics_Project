/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300SKYShader.vert
Purpose: environment vertex shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/25/2021
End Header --------------------------------------------------------*/
#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 camera;
uniform mat4 projection;
uniform mat4 transform;

out vec2 Tex;
out vec3 vertexNormal;
out vec3 vertexPosition;
out vec3 position;
out vec3 normals;
out vec3 FragPos;

void main()
{
    Tex = aTexCoord;
    position = vPosition;
    normals = aNormal;

    gl_Position = projection * camera * transform * vec4(vPosition, 1.0);
    
    FragPos = vec3(transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0));

    vertexPosition = vec4(transform * vec4(vPosition, 1.0)).xyz;
    vertexNormal = normalize(mat3(transpose(inverse(transform))) * aNormal); 
}  
