/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300SKYShader.frag
Purpose: environment fragment shader
Language: GLSL, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 11/25/2021
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

uniform Material material;

uniform vec3 myColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform int curLightNum;
uniform int curLightType;

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

in vec3 FragPos;
in vec2 Tex;

vec3 Direction(vec3 normal, vec3 viewDir, int i);  
vec3 Point(vec3 normal, vec3 fragPos, vec3 viewDir, int i);  
vec3 Spot(vec3 normal, vec3 fragPos, vec3 viewDir, int i);

/////////////////////////////////////////////////////////
out vec4 FragColor;

in vec3 vertexNormal;
in vec3 vertexPosition;
in vec3 position;
in vec3 normals;

uniform vec3 cameraPos;
uniform int mappingType;

uniform sampler2D left;         //0
uniform sampler2D right;        //1
uniform sampler2D bottom;       //2
uniform sampler2D top;          //3
uniform sampler2D front;        //4
uniform sampler2D back;         //5

uniform float eta;
uniform float chromaticAberration;
uniform float fresnelPower;
uniform float refractionM;
uniform bool usePhong;
uniform bool useChromaticAberration;

vec2 cubeUV = {0,0};

vec4 calCub(vec3 position)
{
    vec3 absPosition = abs(position);
    float maxValue = max(absPosition.x, absPosition.y);
	maxValue = max(maxValue, absPosition.z);

    vec3 uv;
    vec2 uv2;

	if(maxValue == absPosition.x)
	{
        if(position.x <= 0.0f)
        {
            uv = vec3(position.zy, absPosition.x) * vec3(1.f, -1.f, 1.f);
            return texture(left, vec2(1.f -(uv.x / uv.z + 1.f) * 0.5f,  (uv.y / uv.z + 1.f) * 0.5f));
        }
        else
        {
            uv = vec3(position.zy, absPosition.x) * vec3(-1.f, -1.f, 1.f);
            return texture(right, vec2(1.f -(uv.x / uv.z + 1.f) * 0.5f,  (uv.y / uv.z + 1) * 0.5f));
        }	
	}
	else if(maxValue == absPosition.y)
	{
        if(position.y <= 0.0f)
        {
            uv = vec3(position.xz, absPosition.y) * vec3(1.f, -1.f, 1.f);
            uv2 = vec2(uv.xy / uv.z + 1.f) * 0.5f;

            uv2.x = 1.f - uv2.y;
            return texture(top, vec2(uv2.y, uv.x));
        }
        else
        {
            uv = vec3(position.xz, absPosition.y) * vec3(1.f, 1.f, 1.f);
            uv2 = vec2(uv.xy / uv.z + 1.f) * 0.5f;

            uv2.y = 1.f - uv2.x;
            return texture(bottom, vec2(uv2.y, uv.x));
        }
	}
	else
	{
        if(position.z <= 0.0f)
        {
            uv = vec3(position.xy, absPosition.z) * vec3(-1.f, -1.f, 1.f);
            return texture(back, vec2(1 -(uv.x / uv.z + 1.f) * 0.5f,  (uv.y / uv.z + 1.f) * 0.5f));
        }
		else
        {
            uv = vec3(position.xy, absPosition.z) * vec3(1.f, -1.f, 1.f);
            return texture(front, vec2(1.f -(uv.x / uv.z + 1.f) * 0.5f, (uv.y / uv.z + 1) * 0.5f));
        }
	}
}

void main()
{
    //////////////phong///////////////////////////
    vec3 lightingDir = normalize(cameraPos - FragPos);

    vec3 phongShading = vec3(0.f,0.f,0.f);
    
    for(int i = 0; i < curLightNum; i++)
    {
        if(lights[i].lightType == 0/*curLightType == 0*/ ) //direction
        {
            phongShading += Direction(vertexNormal, lightingDir, i);
        }
        else if(lights[i].lightType == 1 /*curLightType == 1*/) //point
        {

            phongShading += Point( vertexNormal, FragPos, lightingDir, i);
        }
        else // spot
        {
            phongShading += Spot(vertexNormal, FragPos, lightingDir, i);
        }
    }

    if(isTexture == 0)
    {
        phongShading += gAmbient;
    }
    else
    {
        phongShading += emmisive * gAmbient * coefficient;
    }
    
    float s = (fogMax - distance(vertexPosition, cameraPos)) / (fogMax - fogMin);

    phongShading = s * phongShading + (1 - s) * fog;
    //////////////////////////////////////////////////////////////////////////////
    float RefractR = 0.65 / 0.66;
    float RefractG = 0.67 / 0.66;
    float RefractB = 0.69 / 0.66;

    vec3 viewDir = normalize(vertexPosition - cameraPos); // i
	vec3 reflectPos = (2 * dot(vertexNormal, viewDir) * vertexNormal - viewDir);
    reflectPos.x *= -1.f;

    vec3 reflectColor = calCub(reflectPos).xyz;
    vec3 refractColor;

    float K = 1.0f / refractionM;
    float refractionPos;
    vec3 refractPos = vec3(0.f);

    vec3 refractPosR = vec3(0.f);
    vec3 refractPosG = vec3(0.f);
    vec3 refractPosB = vec3(0.f);

    if(useChromaticAberration == true)
    {
        float K_R = (1.0f / refractionM) * RefractR * chromaticAberration;
        float K_G = (1.0f / refractionM) * RefractG * chromaticAberration;
        float K_B = (1.0f / refractionM) * RefractB * chromaticAberration;

        float refractionPosR = 1.0f - K_R * K_R * (1.0f - dot(vertexNormal, viewDir) * dot(vertexNormal, viewDir));
	    float refractionPosG = 1.0f - K_G * K_G * (1.0f - dot(vertexNormal, viewDir) * dot(vertexNormal, viewDir));
	    float refractionPosB = 1.0f - K_B * K_B * (1.0f - dot(vertexNormal, viewDir) * dot(vertexNormal, viewDir));
    
        if(refractionPosR >= 0)
        {
            refractPosR =  (K_R * (dot(vertexNormal, viewDir)) + sqrt(refractionPosR)) * vertexNormal - K_R * viewDir;
        }

        if(refractionPosG >= 0)
        {
            refractPosG =  (K_G * (dot(vertexNormal, viewDir)) + sqrt(refractionPosG)) * vertexNormal - K_G * viewDir;
        }

        if(refractionPosB >= 0)
        {
            refractPosB =  (K_B * (dot(vertexNormal, viewDir)) + sqrt(refractionPosB)) * vertexNormal - K_B * viewDir;
        }

        refractColor.x = calCub(refractPosR).x;
        refractColor.y = calCub(refractPosG).y;
        refractColor.z = calCub(refractPosB).z;
    }
    else
    {
        K = 1.0f / refractionM;

	    refractionPos = 1.0f - K * K * (1.0f - dot(vertexNormal, viewDir) * dot(vertexNormal, viewDir));

        if(refractionPos >= 0)
        {
           refractPos =  (K * (dot(vertexNormal, viewDir)) + sqrt(refractionPos)) * vertexNormal - K * viewDir;
        }

        refractColor = calCub(refractPos).xyz;
    }

     

    if(mappingType == 0) // only reflaction
    {
        if(usePhong)
        {
            phongShading = mix(reflectColor, phongShading, 0.5f);
            FragColor = vec4(phongShading, 1.0f);
        }
        else
        {
            FragColor = vec4(reflectColor, 1.0f);
        }
        
    }
    else if(mappingType == 1) // only refraction
    {
        if(usePhong)
        {
            phongShading = mix(refractColor, phongShading, 0.5f);
            FragColor = vec4(phongShading, 1.0f);
        }
        else
        {
            FragColor = vec4(refractColor, 1.0f);
        }
    }
    else if(mappingType == 2)
    {    
        float F = ((1.0 - K) * (1.0 - K)) / ((1.0 + K) * (1.0 + K));

        float Ratio = F + (1.0 - F) * pow(1.0 - dot(normalize(cameraPos - vertexPosition), vertexNormal), fresnelPower);
        
        vec3 mixColor = mix(refractColor, reflectColor, Ratio);

        if(usePhong)
        {
            phongShading = mix(mixColor, phongShading, 1.0f);
            FragColor = vec4(phongShading, 1.0f);
        }
        else
        {
            FragColor = vec4(mixColor, 1.0f);
        }      
    }
} 

vec3 Direction(vec3 normal, vec3 viewDir, int i)
{
    vec3 lightDir = normalize(-lights[i].direction);

    vec3 ambients  = lights[i].ambient  /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuses  = lights[i].diffuse  * diff /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), material.shininess);
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
    float attenuation = min(1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance)), 1.0);    

    vec3 ambient  = lights[i].ambient /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = lights[i].diffuse  * diff /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), material.shininess);
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
    float attenuation = min(1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance)), 1.0f);    
        
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

    vec3 ambient = lights[i].ambient /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * diff /* * vec3(texture(material.diffuse, Tex).rgb)*/;

    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), material.shininess);
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