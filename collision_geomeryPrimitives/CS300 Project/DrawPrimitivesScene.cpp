/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DrawScene.cpp
Purpose: draw mesh and load obj file
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 10/15/2021
End Header --------------------------------------------------------*/

#include "DrawPrimitivesScene.h"
#include "Camera.h"
#include <iostream>
#include <glm/detail/type_mat4x4.hpp>

extern Camera camera;

/////////// Primitive
// 
//SphereVsSphere
BoundingSphere sphere(0.2f, { 1.0f,0.8f,1.0f }); 
BoundingSphere sphere2(0.1f, { 0.8f,0.8f,1.0f }); 

// AABBVVsSphere
AABB aabb({ -0.2f,-0.2f ,-0.2f }, { 0.3f,0.3f ,0.3f }); 
BoundingSphere sphere3(0.1f, { -0.1f,-0.1f,-0.1f }); 

// SphereVsAABB
BoundingSphere sphere4(0.5f, { 0.5f,0.5f,0.5f }); 
AABB aabb2({ 0.1f,0.1f ,0.1f }, { 0.4f,0.4f ,0.4f }); 

// AABBVsAABB
AABB aabb3({ -1.8f, -1.8f , -1.8f }, { -1.f, -1.f , -1.f }); 
AABB aabb4({ -1.6f, -1.6f , -1.6f }, { -0.8f, -0.8f , -0.8f });
 
//PointVsSphere
Point3D point({ 0.1f,0.1f,0.1f });
BoundingSphere sphere5(0.5f, { 0.5f,0.5f,0.5f }); 

//PointVsAABB
Point3D point2({ 0.1f,0.1f,0.1f }); 
AABB aabb5({ 0.1f,0.1f ,0.1f }, { 0.4f,0.4f ,0.4f }); 

//PointVsTriangle
Point3D point3({ 0.8f,0.5f,0.5f }); 
Triangle triangle({ 0.5f,0.5f,0.5f }, { 0.8f,0.5f,0.5f }, { 0.5f,0.9f,0.5f }); 

//PointVsPlane
Point3D point4({ 0.5f,0.5f,0.5f }); 
Plane plane({ 0.5f,0.5f,0.5f }, 0.1f);

//RayVsPlane
Ray ray({ 0.3f,0.3f,0.3f }, { -0.2f,-0.2f,-0.2f }); 
Plane plane2({ 0.5f,0.5f,0.5f }, 0.1f); 

//RayVsAABB
Ray ray2({ 0.0f,0.0f,0.0f }, { -0.2f,-0.2f,-0.2f }); 
AABB aabb6({ -0.2f,-0.2f ,-0.2f }, { 0.5f,0.5f ,0.5f });

//RayVsSphere
Ray ray3({ 0.3f,0.3f,0.3f }, { -0.2f,-0.2f,-0.2f }); // not useing, I am only use ray2
BoundingSphere sphere6(0.2f, { 0.6f,0.6f,-0.5f });

//RayVsTriangle
Ray ray4({ 0.3f,0.3f,0.3f }, { -0.2f,-0.2f,-0.2f }); // not useing, I am only use ray2
Triangle triangle2({ -0.5f,0.0f,0.0f }, { 0.5f,0.0f,0.0f }, { 0.0f,0.9f,0.0f }); 

//PlaneVsAABB 
Plane plane3({ 0.5f,0.5f,0.5f }, 0.1f); 
AABB aabb7({ 0.1f,0.1f ,0.1f }, { 0.4f,0.4f ,0.4f }); 

//PlaneVsSphere
Plane plane4({ 0.5f,0.5f,0.5f }, 0.1f); 
BoundingSphere sphere7(0.5f, { 0.5f,0.5f,0.5f }); 

//PlaneVsTriangle
Plane plane5({ 0.5f,0.5f,0.5f }, 0.1f); 
Triangle triangle3({ 0.0f,0.0f,0.0f }, { 0.5f,0.0f,0.0f }, { 0.0f,0.9f,0.0f }); 

DrawPrimitivesScene::DrawPrimitivesScene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
	centerPosition = glm::mat4(1.f);

	winWidth = static_cast<float>(windowWidth);
	winHeight = static_cast<float>(windowHeight);
}

DrawPrimitivesScene::~DrawPrimitivesScene()
{
	CleanUp();
}

void DrawPrimitivesScene::CleanUp()
{
	glDeleteProgram(primitivesProgram);
}

int DrawPrimitivesScene::Init()
{
	primitivesProgram = primitivesShader.LoadShaders("../Common/shaders/CS350Intersection.vert", "../Common/shaders/CS350Intersection.frag");

	centerPosition = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f));
	fog = glm::vec3(230.f / 255.f, 230.f / 255.f, 250.f / 255.f);

	sphere.SetUp();
	sphere2.SetUp();
	sphere3.SetUp();
	sphere4.SetUp();
	sphere5.SetUp();
	sphere6.SetUp();
	sphere7.SetUp();

	aabb.SetUp();
	aabb2.SetUp();
	aabb3.SetUp();
	aabb4.SetUp();
	aabb5.SetUp();
	aabb6.SetUp();
	aabb7.SetUp();

	triangle.SetUp();
	triangle2.SetUp();
	triangle3.SetUp();

	plane.SetUp();
	plane2.SetUp();
	plane3.SetUp();
	plane4.SetUp();
	plane5.SetUp();

	ray.SetUp();
	ray2.SetUp();
	ray3.SetUp();
	ray4.SetUp();

	return Scene::Init();
}

int DrawPrimitivesScene::Render()
{
	/////////////////////////////////////////////////geomatry pass
	glClearColor(fog.x, fog.y, fog.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(primitivesProgram);

	glm::mat4 cameras = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
	glm::mat4 projection = glm::perspective(glm::radians(50.f), winWidth / winHeight, 0.1f, 10.f);
	glm::mat4 transform = centerPosition;

	GLint transformVertex = glGetUniformLocation(primitivesProgram, "transform");
	glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &transform[0][0]);

	GLint cameraVertex = glGetUniformLocation(primitivesProgram, "camera");
	glUniformMatrix4fv(cameraVertex, 1, GL_FALSE, &cameras[0][0]);

	GLint projectionVertex = glGetUniformLocation(primitivesProgram, "projection");
	glUniformMatrix4fv(projectionVertex, 1, GL_FALSE, &projection[0][0]);

	GLint newColor = glGetUniformLocation(primitivesProgram, "newColor");
	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	//glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &planePosition[0][0]);

	/////sphere vs sphere
	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	glUniform3f(newColor, spheresphere.x, spheresphere.y, spheresphere.z);
	glBindVertexArray(sphere.vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere.indices.size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(sphere2.vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere2.indices.size()), GL_UNSIGNED_INT, 0);
	/////

	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	/////aabb vs sphere
	glUniform3f(newColor, aabbsphere.x, aabbsphere.y, aabbsphere.z);

	glBindVertexArray(aabb.vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb.vertex.size()));

	glBindVertexArray(sphere3.vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere3.indices.size()), GL_UNSIGNED_INT, 0);
	/////

	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	/////sphere vs aabb
	//glBindVertexArray(aabb2.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb2.vertex.size()));

	//glBindVertexArray(sphere4.vao);
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere4.indices.size()), GL_UNSIGNED_INT, 0);
	/////
	
	/////aabb vs aabb

	glUniform3f(newColor, aabbaabb.x, aabbaabb.y, aabbaabb.z);
	
	glBindVertexArray(aabb3.vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb3.vertex.size()));

	glBindVertexArray(aabb4.vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb4.vertex.size()));
	/////

	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);
	
	/////point vs sphere
	//glBindVertexArray(point.vao);
	//glDrawArrays(GL_POINT, 0, static_cast<GLsizei>(aabb.vertex.size()));

	//glBindVertexArray(sphere5.vao);
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere5.indices.size()), GL_UNSIGNED_INT, 0);
	/////
	
	/////point vs aabb
	//glBindVertexArray(point2.vao);
	//glDrawArrays(GL_POINT, 0, static_cast<GLsizei>(point2.vertex.size()));

	//glBindVertexArray(aabb5.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb5.vertex.size()));
	/////
	
	/////point vs triangle
	glUniform3f(newColor, pointtriangle.x, pointtriangle.y, pointtriangle.z);

	glBindVertexArray(point3.vao);
	glDrawArrays(GL_POINT, 0, static_cast<GLsizei>(point3.vertex.size()));

	glBindVertexArray(triangle.vao);
	glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(triangle.vertex.size()));
	/////
	
	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	/////point vs plane
	//glBindVertexArray(point4.vao);
	//glDrawArrays(GL_POINT, 0, static_cast<GLsizei>(point4.vertex.size()));

	//glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &plane.modelmatrix[0][0]);
	//glBindVertexArray(plane.vao);
	//glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(plane.vertex.size()));
	/////

	glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &transform[0][0]);

	/////ray vs plane
	//glBindVertexArray(ray.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ray.vertex.size()));

	//glBindVertexArray(plane2.vao);
	//glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(plane2.vertex.size()));
	/////

	/////ray vs aabb
	glUniform3f(newColor, rayaabb.x, rayaabb.y, rayaabb.z);

	glBindVertexArray(ray2.vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ray2.vertex.size()));

	glBindVertexArray(aabb6.vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb6.vertex.size()));
	/////

	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	/////ray vs sphere
	glUniform3f(newColor, raysphere.x, raysphere.y, raysphere.z);
	//glBindVertexArray(ray3.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ray3.vertex.size()));
	glBindVertexArray(sphere6.vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere6.indices.size()), GL_UNSIGNED_INT, 0);
	/////

	glUniform3f(newColor, newColors.x, newColors.y, newColors.z);

	/////ray vs triangle
	glUniform3f(newColor, rayaabb.x, rayaabb.y, rayaabb.z);

	//glBindVertexArray(ray4.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(ray4.vertex.size()));

	glBindVertexArray(triangle2.vao);
	glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(triangle2.vertex.size()));
	/////

	/////plane vs aabb
	//glBindVertexArray(plane3.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(plane3.vertex.size()));

	//glBindVertexArray(aabb7.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(aabb7.vertex.size()));
	/////

	/////plane vs spehre
	//glBindVertexArray(plane4.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(plane4.vertex.size()));

	//glBindVertexArray(sphere7.vao);
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere7.indices.size()), GL_UNSIGNED_INT, 0);
	/////

	/////plane vs triangle
	//glBindVertexArray(plane5.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(plane5.vertex.size()));

	//glBindVertexArray(triangle3.vao);
	//glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(triangle3.vertex.size()));
	/////

	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);

	ImGuiUpdate();

	return 0;
}

int DrawPrimitivesScene::postRender()
{
	if (intersection.SphereVsSphere(sphere, sphere2))
	{
		spheresphere = { 221.f / 255.f,160.f / 255.f,221.f / 255.f };
	}

	if (intersection.AABBVVsSphere(aabb, sphere3))
	{
		aabbsphere = { 255.f / 255.f,240.f / 255.f,245.f / 255.f };
	}

	if (intersection.SphereVsAABB(sphere4, aabb2))
	{
		sphereaabb = { 224.f / 255.f,255.f / 255.f,255.f / 255.f };
	}

	if (intersection.AABBVsAABB(aabb3, aabb4))
	{
		aabbaabb = { 72.f / 255.f,61.f / 255.f, 139.f / 255.f };
	}

	if (intersection.PointVsSphere(point, sphere5))
	{
		pointsphere = { 147.f / 255.f,112.f / 255.f,219.f / 255.f };
	}

	if (intersection.PointVsAABB(point2, aabb5))
	{
		pointaabb = { 255.f / 255.f,255.f / 255.f,0.f / 255.f };
	}

	if (intersection.PointVsTriangle(point3, triangle))
	{
		pointtriangle = { 72.f / 255.f,209.f / 255.f,204.f / 255.f };
	}

	if (intersection.PointVsPlane(point4, plane))
	{
		pointplane = { 127.f / 255.f,255.f / 255.f,212.f / 255.f };
	}

	if (intersection.RayVsPlane(ray, plane2))
	{
		rayplane = { 255.f / 255.f,182.f / 255.f,193.f / 255.f };
	}

	if (intersection.RayVsAABB(ray2, aabb6))
	{
		rayaabb = { 219.f / 255.f,112.f / 255.f,147.f / 255.f };
	}

	if (intersection.RayVsSphere(ray2, sphere6))
	{
		raysphere = { 64.f / 255.f,224.f / 255.f,208.f / 255.f };
	}

	if (intersection.RayVsTriangle(ray2, triangle2))
	{
		raytriagnle = { 216.f / 255.f,191.f / 255.f,216.f / 255.f };
	}

	if (intersection.PlaneVsAABB(plane3, aabb7))
	{
		planeaabb = { 255.f / 255.f,0.f,0.f };
	}

	if (intersection.PlaneVsSphere(plane4, sphere7))
	{
		planesphere = { 189.f / 255.f,183.f / 255.f,107.f / 255.f };
	}

	if (intersection.PlaneVsTriangle(plane5, triangle3))
	{
		planeshpere = { 128.f / 255.f,128.f / 255.f,0.f };
	}

	return 0;
}

void DrawPrimitivesScene::ImGuiUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("ImGui");

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}