/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS350PhongLight.frag
Purpose: Phong shading fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 02/01/2022
End Header --------------------------------------------------------*/

#version 410 core

struct Lights
{
    float constant;   //4  0
    float linear;     //4  4
    float quadratic;  //4  8

    float fallOff;    //4  12
    float outerAngle; //4  16  
    float innerAngle; //4  20

    int lightType;    //4  24

    vec3 position;    //16 32
    vec3 direction;   //16 48
    vec3 ambient;     //16 64
    vec3 diffuse;     //16 80
    vec3 specular;    //16 96
};

 layout (std140) uniform Light
 {   
    Lights lights[16];
 };

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

out vec4 color;
in vec2 tex;

uniform vec3 viewPos;

uniform int renderType;
uniform int currLightNum;

uniform float shininess;

uniform vec3 globalAmbient;
uniform vec3 coefficient;
uniform vec3 emmisive;

uniform vec3 fog;
uniform float fogMin;
uniform float fogMax;

vec3 fragPos;
vec3 Normal;
vec3 diffuse;
float specular;

vec3 Direction(vec3 norm, vec3 viewDir, Lights light);
vec3 Point(vec3 norm, vec3 viewDir,Lights light);
vec3 Spot(vec3 norm, vec3 viewDir,Lights light);

void main()
{
    vec3 result;     

    fragPos = texture(gPosition, tex).rgb;
    Normal = texture(gNormal, tex).rgb;
    diffuse = texture(gAlbedoSpec, tex).rgb;
    specular = texture(gAlbedoSpec, tex).a;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    for(int i = 0; i < currLightNum; i++)
    {
        if(lights[i].lightType == 0) //direction
        {
            result += Direction(norm, viewDir, lights[i]);         
        }
        else if(lights[i].lightType == 1) //point
        {
            result += Point(norm, viewDir, lights[i]);  
        }
        else if(lights[i].lightType == 2) //Spot
        {
            result += Spot(norm, viewDir, lights[i]);       
        }
    }

    result += globalAmbient * emmisive * coefficient;

    float s = (fogMax - length(viewPos)) / (fogMax - fogMin);
    result = s * result + (1 - s) * fog;
    
    if(renderType == 0)
    {
        color = vec4(result, 1.0);
    }
    else if(renderType == 1)
    {
        color = vec4(fragPos, 1.0);
    }
    else if(renderType == 2)
    {
        color = vec4(Normal, 1.0);
    }
    else if(renderType == 3)
    {
        color = vec4(diffuse, 1.0);
    }
    else if(renderType == 4)
    {
        color = vec4(vec3(specular),1.0);
    }
}

vec3 Direction(vec3 norm, vec3 viewDir, Lights light)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambients = light.ambient * diffuse;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuses = light.diffuse * diff * diffuse;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), shininess);
    vec3 speculars = light.specular * spec * specular;

    vec3 result = (ambients + diffuses + speculars);

    return result;
}

vec3 Point(vec3 norm, vec3 viewDir, Lights light)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float distance = length(light.position - fragPos);
    float attenuation = min(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1.0);
    
    vec3 ambients = light.ambient * diffuse;   

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuses = light.diffuse * diff * diffuse;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), shininess);
    vec3 speculars = light.specular * spec * specular;
     
    ambients *= attenuation;
    diffuses *= attenuation;
    speculars *= attenuation; 
   
    vec3 result = (ambients + diffuses + speculars);

    return result;
}

vec3 Spot(vec3 norm, vec3 viewDir, Lights light)
{
    float mRadian = 3.141592f/180.f;
    vec3  lightDir = normalize(light.position - fragPos);

    //attenuation
    float SpotEfect = 1.0f;
    float distance = length(light.position-fragPos);
    float attenuation = min(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1.0f);    

    float theta = dot(lightDir, normalize(-light.direction)); 

    float innerAngles = light.innerAngle * mRadian;
    float outerAngles = light.outerAngle * mRadian;

    if(theta > cos(innerAngles))
    {
        SpotEfect = 1.f;
    }
    else if(theta < cos(outerAngles))
    {
        SpotEfect = 0.f;
    }
    else
    {
        SpotEfect = pow((theta - cos(outerAngles)) / (cos(innerAngles) - cos(outerAngles)), light.fallOff);
    }
    
    vec3 ambients = light.ambient * diffuse;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3  diffuses = light.diffuse * diff * diffuse;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), shininess);
    vec3  speculars = light.specular * spec * specular;    

    ambients *= attenuation;
    diffuses *= attenuation * SpotEfect;
    speculars *= attenuation * SpotEfect; 
    
    vec3 result = (ambients + diffuses + speculars);

    return result;
}
