/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS350gBuufer.frag
Purpose:gbuffer fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 02/01/2022
End Header --------------------------------------------------------*/

#version 410 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 fragPos;
in vec3 normal;
in vec2 tex;

uniform sampler2D diffuse;
uniform sampler2D specular; 

void main()
{
    gPosition = fragPos;

    gNormal = normal;
    
    gAlbedoSpec.rgb = texture(diffuse, tex).rgb;
    
    gAlbedoSpec.a = texture(specular, tex).r;
}