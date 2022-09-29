/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300ShaderBlinnShading.vert
Purpose: vertex shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/01/2021
End Header --------------------------------------------------------*/
#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 camera;
uniform mat4 projection;
uniform mat4 transform;

out vec3 Normal;
out vec3 FragPos;
out vec2 Tex;
out vec3 World;
out vec3 normals;
out vec3 positions;

void main()
{
	FragPos = vec3(transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0));

	Normal = mat3(transpose(inverse(transform)))* aNormal;
	normals = aNormal;
	positions = vPosition;
	World = vec4(transform * vec4(vPosition, 1.0)).xyz;
	mat4 viewMatrix = projection * camera;

	gl_Position = viewMatrix * transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0f);
	Tex = vec2(aTexCoord.x, aTexCoord.y);
}
