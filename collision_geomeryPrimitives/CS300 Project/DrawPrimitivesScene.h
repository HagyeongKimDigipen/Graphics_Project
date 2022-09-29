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
#include "Camera.h"
#include "AABB.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include "Point3D.h"
#include "Ray.h"
#include "Triangle.h"
#include "Intersection.h"

class DrawPrimitivesScene : public Scene
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

	DrawPrimitivesScene() = default;
	DrawPrimitivesScene(int windowWidth, int windowHeight);

	virtual ~DrawPrimitivesScene();

	int Init() override;
	void CleanUp() override;

	int Render() override;
	int postRender() override;

	void ImGuiUpdate();

	//////////////////////////////////////////////////////////////////
	MyColor myColor;
	glm::vec3 fog;

	glm::vec3 newColors = { 0.f,0.f,0.f };

	glm::vec3 spheresphere = { 0.f,0.f,0.f };
	glm::vec3 aabbsphere = { 0.f,0.f,0.f };
	glm::vec3 sphereaabb = { 0.f,0.f,0.f };
	glm::vec3 aabbaabb = { 0.f,0.f,0.f };
	glm::vec3 pointsphere = { 0.f,0.f,0.f };
	glm::vec3 pointaabb = { 0.f,0.f,0.f };
	glm::vec3 pointtriangle = { 0.f,0.f,0.f };
	glm::vec3 pointplane = { 0.f,0.f,0.f };
	glm::vec3 rayplane = { 0.f,0.f,0.f };
	glm::vec3 rayaabb = { 0.f,0.f,0.f };
	glm::vec3 raysphere = { 0.f,0.f,0.f };
	glm::vec3 raytriagnle = { 0.f,0.f,0.f };
	glm::vec3 planeaabb = { 0.f,0.f,0.f };
	glm::vec3 planesphere = { 0.f,0.f,0.f };
	glm::vec3 planeshpere = { 0.f,0.f,0.f };

	/// ImGui


	/// obj
	float winWidth;
	float winHeight;
	glm::mat4 centerPosition;

	//////////////////////// 
	Shader primitivesShader;
	GLuint primitivesProgram;

	///camera
	glm::vec3 cameraPosition = glm::vec3(0.f, 0.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float cameraSpeed = 0.01f;

	Intersection intersection;
};