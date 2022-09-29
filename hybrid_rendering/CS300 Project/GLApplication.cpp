/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GLApplication.cpp
Purpose: draw window, scene and control camera
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 09/23/2021
End Header --------------------------------------------------------*/

#include <Windows.h>
#include <cstdio>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "DrawScene.h"
//////////////////////////////////////////////////////////////////////
GLFWwindow* window;

int Widthwindow = 1024;
int Heightwindow = 800;

DrawScene* myScene;
Camera camera = Camera(glm::vec3(0.f, 1.5f, 5.0f));

//////mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Framebuffer_resize_callback(GLFWwindow* /*window*/, int width, int height);

float lastX = 512.f;
float lastY = 400.f;

bool first = true;

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		(void)getchar();
		return -1;
	}

	// Setting up OpenGL properties
	glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(Widthwindow, Heightwindow, "cs350_Assignment1", nullptr, nullptr);

	if (window == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window. Check if your GPU is 4.0 compatible.\n");
		(void)getchar();
		glfwTerminate();
		return -1;
	}
	glfwGetFramebufferSize(window, &Widthwindow, &Heightwindow);
	glfwSetFramebufferSizeCallback(window, Framebuffer_resize_callback);

	glViewport(0, 0, Widthwindow, Heightwindow);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = static_cast<GLboolean>(true);
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		(void)getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Initialize the scene
	myScene = new DrawScene(Widthwindow, Heightwindow);

	// Scene::Init encapsulates setting up the geometry and the texture
	// information for the scene
	myScene->Init();

	//////////ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); 
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui_ImplOpenGL3_Init("#version 410");

	float cameraSpeed = 0.01f;

	do
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(0, cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(1, cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(2, cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(3, cameraSpeed);
		}

		myScene->Display();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	myScene->CleanUp();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (first)
		{
			lastX = static_cast<float>(xpos);
			lastY = static_cast<float>(ypos);
			first = false;
		}

		float xoffset = static_cast<float>(xpos) - lastX;
		float yoffset = lastY - static_cast<float>(ypos);

		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

		float sensitivity = 0.05f;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.Yaw += xoffset;
		camera.Pitch += yoffset;

		if (camera.Pitch > 89.0f)
		{
			camera.Pitch = 89.0f;
		}
		if (camera.Pitch < -89.0f)
		{
			camera.Pitch = -89.0f;
		}

		glm::vec3 front;

		front.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
		front.y = sin(glm::radians(camera.Pitch));
		front.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));

		camera.Front = glm::normalize(front);
	}
}

void Framebuffer_resize_callback(GLFWwindow* /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
}