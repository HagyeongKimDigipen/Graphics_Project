/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300ShaderBlinnShading.frag
Purpose: fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/01/2021
End Header --------------------------------------------------------*/
#version 410 core

struct Material {
    sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 

struct Lights
{
    float constant; //4  0
    float linear;   //4  4
    float quadratic;  //4 8

    float fallOff;   //4   12
    float outerAngle; //4  16  
    float innerAngle;  //4 20

    int lightType;    //4 24

    vec3 position;  //16 32
    vec3 direction; //16 48
    vec3 ambient;   //16 64
    vec3 diffuse;   //16 80
    vec3 specular;  //16 96
};

layout (std140) uniform Lightss
{
    Lights lights[16];
};

///////////////////////////
out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 Tex;
in vec3 World;

uniform vec3 myColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform int curLightNum;
uniform int curLightType;

////
uniform int checkUV;
uniform int GPUcalculation;
uniform int checkEntity;

uniform vec3 emmisive;
uniform vec3 coefficient;
uniform vec3 gAmbient;

uniform vec3 uvMin;
uniform vec3 uvMax;

uniform vec3 fog;
uniform float fogMin;
uniform float fogMax;

uniform int isTexture;

//// struct instance
uniform Material material;

//// light function
vec3 Direction(vec3 normal, vec3 viewDir, int i);  
vec3 Point(vec3 normal, vec3 fragPos, vec3 viewDir, int i);  
vec3 Spot(vec3 normal, vec3 fragPos, vec3 viewDir, int i);

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.f,0.f,0.f);

    for(int i = 0; i < curLightNum; i++)
    {
        if(lights[i].lightType == 0/*curLightType == 0*/ ) //direction
        {
            result += Direction(norm, viewDir, i);
        }
        else if(lights[i].lightType == 1 /*curLightType == 1*/) //point
        {

            result += Point( norm, FragPos, viewDir, i);
        }
        else // spot
        {
            result += Spot(norm, FragPos, viewDir, i);
        }
    }

    if(isTexture == 0)
    {
        result += gAmbient;
    }
    else
    {
        result += emmisive * gAmbient * coefficient;
    }

    float s = (fogMax - distance(viewPos, World)) / (fogMax - fogMin);
    result = s * result + (1 - s) * fog;
    color = vec4(result, 1.0f);
}

vec3 Direction(vec3 normal, vec3 viewDir, int i)
{
    vec3 lightDir = normalize(-lights[i].direction);

    vec3 ambients  = lights[i].ambient  * vec3(texture(material.diffuse, Tex).rgb);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuses  = lights[i].diffuse  * diff * vec3(texture(material.diffuse, Tex).rgb);
    vec3 reflectionHarlf = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, reflectionHarlf), 0.0), material.shininess);
    vec3 speculars = lights[i].specular * spec * vec3(texture(material.specular, Tex).rgb);

    if(isTexture == 0)
    {
        ambients  = lights[i].ambient;
        diffuses  = lights[i].diffuse  * diff;
        speculars = lights[i].specular * spec;
    }

    vec3 result = (ambients + diffuses + speculars);

    return result;
} 

vec3 Point(vec3 normal, vec3 fragPos, vec3 viewDir, int i)
{
    vec3 lightDir = normalize(lights[i].position - fragPos);

    // attenuation
    float distance    = length(lights[i].position - fragPos);
    float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));    

    vec3 ambient  = lights[i].ambient  * vec3(texture(material.diffuse, Tex).rgb);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = lights[i].diffuse  * diff * vec3(texture(material.diffuse, Tex).rgb);
    vec3 reflectionHarlf = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, reflectionHarlf), 0.0), material.shininess);
    vec3 specular = lights[i].specular * spec * vec3(texture(material.specular, Tex).rgb);

    if(isTexture == 0)
    {
        ambient  = lights[i].ambient;
        diffuse  = lights[i].diffuse  * diff;
        specular = lights[i].specular * spec;
    }

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);

    return result;
} 

vec3 Spot(vec3 normal, vec3 fragPos, vec3 viewDir, int i)
{
    float mRadian = 3.141592/180;
    vec3 lightDir = normalize(lights[i].position - fragPos);

    //attenuation
    float spotlightEfect = 0.f;
    float distance = length(lights[i].position - fragPos);
    float attenuation = min(1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance)), 1.0);    
        
    float theta = dot(lightDir, normalize(-lights[i].direction)); 
    
    float innerAngles = lights[i].innerAngle * mRadian;
    float outerAngles = lights[i].outerAngle * mRadian;

    if(theta > cos(innerAngles))
    {
        spotlightEfect = 1.f;
    }
    else if(theta < cos(outerAngles))
    {
        spotlightEfect = 0.f;
    }
    else
    {
        spotlightEfect = pow((theta - cos(outerAngles)) / (cos(innerAngles) - cos(outerAngles)), lights[i].fallOff);
    }

    vec3 ambient = lights[i].ambient * vec3(texture(material.diffuse, Tex).rgb);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * diff * vec3(texture(material.diffuse, Tex).rgb);

    vec3 reflectionHarlf = normalize(viewDir + lightDir);

    float spec = pow(max(dot(viewDir, reflectionHarlf), 0.0), material.shininess);
    vec3 specular = lights[i].specular * spec * vec3(texture(material.specular, Tex).rgb);
    
    if(isTexture == 0)
    {
        ambient  = lights[i].ambient;
        diffuse  = lights[i].diffuse  * diff;
        specular = lights[i].specular * spec;
    }

    ambient *= attenuation;
    diffuse *= attenuation * spotlightEfect;
    specular *= attenuation * spotlightEfect;
    
    vec3 result = (ambient + diffuse + specular);

    return result;
}