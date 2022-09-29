/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DrawScene.h
Purpose: draw mesh and load obj file
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/15/2021
End Header --------------------------------------------------------*/
#pragma once

#include "Scene.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shader.hpp"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "Camera.h"

class DrawScene : public Scene
{
public:
	struct MyColor/////// personal color
	{
		glm::vec3 plum = { 221.f / 255.f,160.f / 255.f,221.f / 255.f };
		glm::vec3 lightcyan = { 224.f / 255.f,255.f / 255.f,255.f / 255.f };
		glm::vec3 lavenderblush = { 255.f / 255.f,240.f / 255.f,245.f / 255.f };
		glm::vec3 darkslateblue = { 72.f / 255.f,61.f / 255.f, 139.f / 255.f };
		glm::vec3 mediumpruple = { 147.f / 255.f,112.f / 255.f,219.f / 255.f };
		glm::vec3 yellow = { 255.f / 255.f,255.f / 255.f,0.f / 255.f };
		glm::vec3 mediumturquoise = { 72.f / 255.f,209.f / 255.f,204.f / 255.f };
		glm::vec3 aquamarine = { 127.f / 255.f,255.f / 255.f,212.f / 255.f };
		glm::vec3 lightpink = { 255.f / 255.f,182.f / 255.f,193.f / 255.f };
		glm::vec3 palevioletred = { 219.f / 255.f,112.f / 255.f,147.f / 255.f };
		glm::vec3 turquoise = { 64.f / 255.f,224.f / 255.f,208.f / 255.f };
		glm::vec3 thistle = { 216.f / 255.f,191.f / 255.f,216.f / 255.f };
		glm::vec3 red = { 255.f / 255.f,0.f,0.f };
		glm::vec3 darkkhaki = { 189.f / 255.f,183.f / 255.f,107.f / 255.f };
		glm::vec3 silver = { 192.f / 255.f,192.f / 255.f,192.f / 255.f };
		glm::vec3 olive = { 128.f / 255.f,128.f / 255.f,0.f };
	};

	DrawScene() = default;
	DrawScene(int windowWidth, int windowHeight);

	virtual ~DrawScene();

	int Init() override;
	void CleanUp() override;

	int Render() override;
	int postRender() override;

	void orbitSetUp();
	void sphereSetUp();

	void uniformLight();
	void lightSetUp();

	void normalBufferSetUp();
	void normalFaceBufferSetUp();
	void gBufferSetUp();
	void fsqSetUp();
	void copyDepthInfo();
	void renderDebugObject();
	void ImGuiUpdate();

	//////////////////////////////////////////////////////////////////

	/// gbuffer
	GLuint GBuffer;
	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedo;

	float fsq[20] = { -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, };

	///shader program
	Shader myShader;

	GLuint gBufferProgram;
	GLuint lightProgram;
	GLuint normalProgram;

	/// buffer
	GLuint spherevao;
	GLuint spherenorm;
	GLuint orbitvao;
	GLuint Planeubo;

	GLuint fsqvao;
	GLuint fsqvbo;
	GLuint fsqnorm;
	GLuint fsquv;

	GLuint normalvao;
	GLuint normalvbo;
	GLuint normalebo;

	GLuint normalFacevao;
	GLuint normalFacevbo;
	GLuint normalFaceebo;

	Texture texture;
	GLuint textureDiffuse;
	GLuint textureSpecular;

	/// light
	GLuint uniformBuffer;
	int currLightNum = 1;

	int lightType[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

	float constant = 1.0f;
	float linear = 0.1f;
	float quadratic = 0.03f;

	float outerAngle[16] = { 20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f };
	float innerAngle[16] = { 10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f };
	float falloff[16] = { 1.0f, 1.0f ,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };

	glm::vec3 lightDir[16];
	glm::vec3 lightPosition[16];
	glm::vec3 ambients[16] = { {1.f, 1.f, 1.f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f} };
	glm::vec3 diffuses[16] = { {1.f, 1.f, 1.f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f},{0.8f, 0.6f, 0.6f },{0.1f, 0.7f, 0.7f} };
	glm::vec3 speculars[16] = { {1.f, 1.f, 1.f},{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} };

	glm::vec3 emmissive;
	glm::vec3 globalAmbient;
	glm::vec3 coefficient;

	glm::vec3 fog;
	float fogMax;
	float fogMin;

	///draw
	float radius = 0.05f;
	const float PI = 3.141592f;
	GLfloat angleOfRotation;

	Mesh sphere;
	Mesh orbit;

	glm::mat4 centerPosition;
	//glm::mat4 planePosition;
	glm::mat4 orbitPosition1;

	glm::vec3 min;
	glm::vec3 max;

	MyColor myColor;

	/// ImGui
	bool isVertexNormal;
	bool isFaceNormal;

	int centerObject = 0;

	int renderType = 0;
	bool depth;

	const char* objFile[3] = { "bunny", "cube", "sphere" };
	int obj = 0;

	const char* lightTypes[3] = { "direction", "point", "spot" };
	int lightTypeChange = 1;

	const char* renders[5] = { "Original", "position", "normal", "albedo", "specular" };
	int renderTypes = 0;

	/// obj
	OBJMesh* myMesh;
	//OBJMesh* myMesh2;

	float winWidth;
	float winHeight;

	//////////////////////// 
	
	///camera
	glm::vec3 cameraPosition = glm::vec3(0.f, 0.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float cameraSpeed = 0.01f;
};
