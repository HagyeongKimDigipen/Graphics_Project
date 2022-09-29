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

#include "DrawScene.h"
#include "Camera.h"
#include <iostream>
#include <glm/detail/type_mat4x4.hpp>

extern Camera camera;

DrawScene::DrawScene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
	angleOfRotation = 0.f;

	isVertexNormal = false;
	isFaceNormal = false;
	depth = true;

	centerPosition = glm::mat4(1.f);
	orbitPosition1 = glm::mat4(1.f);

	winWidth = static_cast<float>(windowWidth);
	winHeight = static_cast<float>(windowHeight);
}

DrawScene::~DrawScene()
{
	CleanUp();
}

void DrawScene::CleanUp()
{
	glDeleteProgram(normalProgram);
	glDeleteProgram(lightProgram);
	glDeleteProgram(gBufferProgram);

	glDeleteBuffers(1, &spherevao);
	glDeleteBuffers(1, &spherenorm);
	glDeleteBuffers(1, &orbitvao);
	glDeleteBuffers(1, &Planeubo);
	glDeleteBuffers(1, &fsqvao);
	glDeleteBuffers(1, &fsqvbo);
	glDeleteBuffers(1, &fsqnorm);
	glDeleteBuffers(1, &fsquv);
	glDeleteBuffers(1, &normalvao);
	glDeleteBuffers(1, &normalvbo);
	glDeleteBuffers(1, &normalebo);
	glDeleteBuffers(1, &normalebo);
	glDeleteBuffers(1, &normalFacevao);
	glDeleteBuffers(1, &normalFacevbo);
	glDeleteBuffers(1, &normalFaceebo);
	glDeleteBuffers(1, &textureDiffuse);
	glDeleteBuffers(1, &textureSpecular);

	delete myMesh;
	//delete myMesh2;
}

int DrawScene::Init()
{
	normalProgram = myShader.LoadShaders("../Common/shaders/CS350normal.vert", "../Common/shaders/CS350normal.frag");
	gBufferProgram = myShader.LoadShaders("../Common/shaders/CS350gBuffer.vert", "../Common/shaders/CS350gBuffer.frag");
	lightProgram = myShader.LoadShaders("../Common/shaders/CS350phongShader.vert", "../Common/shaders/CS350phongShader.frag");

	myMesh = new OBJMesh("../Common/models/bunny.obj");
	myMesh->setupBuffer();

	//myMesh2 = new OBJMesh("../Common/models/quad.obj");
	//myMesh2->setupBuffer();

	globalAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	emmissive = glm::vec3(0.1f, 0.1f, 0.1f);
	coefficient = glm::vec3(0.1f, 0.1f, 0.1f);

	fog = glm::vec3(230.f / 255.f, 230.f / 255.f, 250.f / 255.f);
	fogMin = 2.f;
	fogMax = 30.f;

	min = myMesh->getMesh().boundingBox[0];
	max = myMesh->getMesh().boundingBox[1];

	//planePosition = glm::translate(glm::vec3(0.f, -0.5f, 0.f)) * glm::rotate(1.5f, glm::vec3(1.f, 0.f, 0.f)) * glm::scale(glm::vec3(5.f, 5.f, 5.f));
	
	orbitSetUp();
	sphereSetUp();

	normalBufferSetUp();
	normalFaceBufferSetUp();

	glm::vec3 orbitcenter = orbit.getModelCentroid();
	glm::vec3 sphereCentroid = sphere.getModelCentroid();

	centerPosition = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f));

	gBufferSetUp();
	fsqSetUp();

	textureDiffuse = texture.LoadTexture("../Common/textures/metal_roof_diff_512x512.png", 0);
	textureSpecular = texture.LoadTexture("../Common/textures/metal_roof_spec_512x512.png", 0);

	////////////////light uniform setup

	GLuint uniformLight = glGetUniformBlockIndex(gBufferProgram, "Light");

	glUniformBlockBinding(gBufferProgram, uniformLight, 0);

	glGenBuffers(1, &uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 16 * 112, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBuffer, 0, 16 * 112);

	////////////////
	glUseProgram(lightProgram);

	GLint gPosition = glGetUniformLocation(lightProgram, "gPosition");
	glUniform1i(gPosition, 0);

	GLint gNormal = glGetUniformLocation(lightProgram, "gNormal");
	glUniform1i(gNormal, 1);

	GLint gAlbedoSpec = glGetUniformLocation(lightProgram, "gAlbedoSpec");
	glUniform1i(gAlbedoSpec, 2);

	////////////////
	glEnable(GL_DEPTH_TEST);

	return Scene::Init();
}

int DrawScene::Render()
{
	orbitPosition1 = glm::rotate(angleOfRotation, glm::vec3(0.f, 1.f, 0.f)) * glm::translate(glm::vec3{ 0.f, 0.5f, 0.f }) * glm::translate(glm::vec3(1.f, 0.f, 0.f)) * glm::translate(sphere.getModelCentroid());
	
	lightPosition[0].x = orbitPosition1[3].x;
	lightPosition[0].y = orbitPosition1[3].y;
	lightPosition[0].z = orbitPosition1[3].z;

	/////////////////////////////////////////////////geomatry pass

	glClearColor(fog.x, fog.y, fog.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gBufferProgram);

	glm::mat4 cameras = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
	glm::mat4 projection = glm::perspective(glm::radians(50.f), winWidth / winHeight, 0.1f, 10.f);
	glm::mat4 transform = centerPosition;

	GLint diffuse = glGetUniformLocation(gBufferProgram, "diffuse");
	glUniform1i(diffuse, 0);

	GLint specular = glGetUniformLocation(gBufferProgram, "specular");
	glUniform1i(specular, 1);

	GLint transformVertex = glGetUniformLocation(gBufferProgram, "transform");
	glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &transform[0][0]);

	GLint cameraVertex = glGetUniformLocation(gBufferProgram, "camera");
	glUniformMatrix4fv(cameraVertex, 1, GL_FALSE, &cameras[0][0]);

	GLint projectionVertex = glGetUniformLocation(gBufferProgram, "projection");
	glUniformMatrix4fv(projectionVertex, 1, GL_FALSE, &projection[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(myMesh->vao);
	glDrawElements(GL_TRIANGLES, myMesh->getMesh().getIndexBufferSize(), GL_UNSIGNED_INT, 0);

	//glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &planePosition[0][0]);

	//glBindVertexArray(myMesh2->vao);
	//glDrawElements(GL_TRIANGLES, myMesh2->getMesh().getIndexBufferSize(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(spherevao);
	glDrawElements(GL_TRIANGLES, sphere.getIndexBufferSize(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/////////////////////////////////////////////////light pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(lightProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);

	lightSetUp();

	glBindVertexArray(fsqvao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	////////////////
	glBindVertexArray(0);

	///////////

	if (depth == true)
	{
		copyDepthInfo();
	}

	renderDebugObject();

	ImGuiUpdate();

	return 0;
}

int DrawScene::postRender()
{
	angleOfRotation += 0.01f;
	return 0;
}

void DrawScene::orbitSetUp()
{
	glm::vec3 first = glm::vec3(0.1f);
	glm::vec3 last = glm::vec3(0.1f);

	for (float angle = 0; angle <= 2 * PI; angle += PI / 20.f)
	{
		first = { radius * cos(angle) / 2, 0, radius * sin(angle) / 2 };
		last = { radius * cos(angle + PI / 20.f) / 2, 0, radius * sin(angle + PI / 20.f) / 2 };

		orbit.vertexBuffer.push_back(first);
		orbit.vertexBuffer.push_back(last);
	}

	GLuint vbo;
	GLuint ebo;
	GLuint norm;

	glGenVertexArrays(1, &orbitvao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &norm);
	glGenBuffers(1, &ebo);

	glBindVertexArray(orbitvao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(orbit.getVertexBufferSize()) * 3 * sizeof(GLfloat), orbit.getVertexBuffer(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(orbit.getVertexBufferSize()) * 3 * sizeof(GLfloat), orbit.getVertexNormals(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, orbit.getIndexBufferSize() * sizeof(GLuint), orbit.getIndexBuffer(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, norm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}

void DrawScene::sphereSetUp()
{
	float stack = 20.f;
	float start = 0.f;
	float sector = 20.f;

	float x, y, z;

	float sectorAngle, stackAngle;

	glm::vec3 nvertices(0.1f, 0.1f, 0.1f);

	for (float i = start; i <= stack; ++i)
	{
		stackAngle = PI / 2 - i * (PI / stack);
		z = radius * sinf(stackAngle);

		for (float j = 0; j <= sector; ++j)
		{
			sectorAngle = j * (2 * PI / sector);
			x = radius * cosf(stackAngle) * cosf(sectorAngle);
			y = radius * cosf(stackAngle) * sinf(sectorAngle);

			sphere.vertexBuffer.push_back({ x,y,z });

			nvertices.x = x * 1.0f;
			nvertices.y = y * 1.0f;
			nvertices.z = z * 1.0f;

			sphere.vertexNormals.push_back(nvertices);
		}
	}

	GLint first, second;

	for (float i = 0; i < stack; ++i)
	{
		first = static_cast<GLint>(i * (sector + 1));
		second = static_cast<GLint>(first + sector + 1);

		for (int j = 0; j < sector; ++j, ++first, ++second)
		{
			if (i != 0)
			{
				sphere.vertexIndices.push_back(first);
				sphere.vertexIndices.push_back(second);
				sphere.vertexIndices.push_back(first + 1);
			}

			if (i != (stack - 1.f))
			{
				sphere.vertexIndices.push_back(first + 1);
				sphere.vertexIndices.push_back(second);
				sphere.vertexIndices.push_back(second + 1);
			}
		}
	}

	//////////////////////////////////////////

	GLuint vbo;
	GLuint ebo;
	GLuint SphereNorm;

	glGenVertexArrays(1, &spherevao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &SphereNorm);
	glGenBuffers(1, &ebo);

	glBindVertexArray(spherevao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sphere.getVertexBufferSize()) * 3 * sizeof(GLfloat), sphere.getVertexBuffer(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, SphereNorm);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sphere.getVertexBufferSize()) * 3 * sizeof(GLfloat), sphere.getVertexNormals(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexBufferSize() * sizeof(GLuint), sphere.getIndexBuffer(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, SphereNorm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}

void DrawScene::normalBufferSetUp()
{
	glGenVertexArrays(1, &normalvao);

	glGenBuffers(1, &normalvbo);
	glGenBuffers(1, &normalebo);

	glBindVertexArray(normalvao);

	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);

	glBufferData(GL_ARRAY_BUFFER, myMesh->getMesh().vertexNormalDisplay.size() * 3 * sizeof(GLfloat), myMesh->getMesh().vertexNormalDisplay.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh->getMesh().vertexIndices.size() * sizeof(GLfloat), myMesh->getMesh().vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}

void DrawScene::normalFaceBufferSetUp()
{
	glGenVertexArrays(1, &normalFacevao);

	glGenBuffers(1, &normalFacevbo);
	glGenBuffers(1, &normalFaceebo);

	glBindVertexArray(normalFacevao);

	glBindBuffer(GL_ARRAY_BUFFER, normalFacevbo);

	glBufferData(GL_ARRAY_BUFFER, myMesh->getMesh().faceNormalsDisplay.size() * 3 * sizeof(GLfloat), myMesh->getMesh().faceNormalsDisplay.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalFaceebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh->getMesh().vertexIndices.size() * sizeof(GLfloat), myMesh->getMesh().vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalFacevbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindVertexArray(0);
}

void DrawScene::gBufferSetUp()
{
	glGenFramebuffers(1, &GBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer);

	GLuint rbo;
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	GLuint attaches[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glDrawBuffers(3, attaches);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DrawScene::fsqSetUp()
{
	glGenVertexArrays(1, &fsqvao);

	glGenBuffers(1, &fsqvbo);
	glGenBuffers(1, &fsqnorm);
	glGenBuffers(1, &fsquv);

	glBindVertexArray(fsqvao);

	glBindBuffer(GL_ARRAY_BUFFER, fsqvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fsq), &fsq, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, fsqnorm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fsq), &fsq, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, fsquv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fsq), &fsq, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, fsqvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, fsqnorm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, fsquv);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void DrawScene::copyDepthInfo()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight), 0, 0, static_cast<GLsizeiptr>(winWidth), static_cast<GLsizeiptr>(winHeight), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DrawScene::renderDebugObject()
{
	glUseProgram(normalProgram);

	glm::mat4 transform = centerPosition;
	glm::mat4 cameras = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
	glm::mat4 projection = glm::perspective(glm::radians(50.f), winWidth / winHeight, 0.1f, 10.f);

	GLint transformVertex = glGetUniformLocation(normalProgram, "transform");
	glUniformMatrix4fv(transformVertex, 1, GL_FALSE, &transform[0][0]);

	GLint cameraVertex = glGetUniformLocation(normalProgram, "camera");
	glUniformMatrix4fv(cameraVertex, 1, GL_FALSE, &cameras[0][0]);

	GLint projectionVertex = glGetUniformLocation(normalProgram, "projection");
	glUniformMatrix4fv(projectionVertex, 1, GL_FALSE, &projection[0][0]);

	GLint colorVertex = glGetUniformLocation(normalProgram, "newColor");

	if (isVertexNormal == true)
	{
		glUniform3f(colorVertex, myColor.aquamarine.x, myColor.aquamarine.y, myColor.aquamarine.z);
		glBindVertexArray(normalvao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(myMesh->getMesh().vertexNormalDisplay.size() * 2));
		glBindVertexArray(0);
	}
	if (isFaceNormal == true)
	{
		glUniform3f(colorVertex, myColor.lightpink.x, myColor.lightpink.y, myColor.lightpink.z);
		glBindVertexArray(normalFacevao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(myMesh->getMesh().faceNormalsDisplay.size() * 2));
		glBindVertexArray(0);
	}
}

void DrawScene::uniformLight()
{
	for (int i = 0; i < currLightNum; i++)
	{
		lightDir[i] = -lightPosition[i];

		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112), sizeof(float), &constant);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 4, sizeof(float), &linear);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 8, sizeof(float), &quadratic);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 12, sizeof(float), &falloff[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 16, sizeof(float), &outerAngle[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 20, sizeof(float), &innerAngle[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 24, sizeof(float), &lightType[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 32, sizeof(glm::vec3), &lightPosition[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 48, sizeof(glm::vec3), &lightDir[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 64, sizeof(glm::vec3), &ambients[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 80, sizeof(glm::vec3), &diffuses[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 96, sizeof(glm::vec3), &speculars[i]);
	}
}

void DrawScene::lightSetUp()
{
	uniformLight();

	GLint viewPoss = glGetUniformLocation(lightProgram, "viewPos");
	glUniform3f(viewPoss, camera.Position.x, camera.Position.y, camera.Position.z);

	GLint lightNumPos = glGetUniformLocation(lightProgram, "currLightNum");
	glUniform1i(lightNumPos, currLightNum);

	GLint typePos = glGetUniformLocation(lightProgram, "renderType");
	glUniform1i(typePos, renderType);

	GLint shininess = glGetUniformLocation(lightProgram, "shininess");
	glUniform1f(shininess, 32.0f);

	GLint globalAmbientPos = glGetUniformLocation(lightProgram, "globalAmbient");
	glUniform3f(globalAmbientPos, globalAmbient.x, globalAmbient.y, globalAmbient.z);

	GLint KaPos = glGetUniformLocation(lightProgram, "coefficient");
	glUniform3f(KaPos, coefficient.x, coefficient.y, coefficient.z);

	GLint EmissivePos = glGetUniformLocation(lightProgram, "emmisive");
	glUniform3f(EmissivePos, emmissive.x, emmissive.y, emmissive.z);

	GLint FogPos = glGetUniformLocation(lightProgram, "fog");
	glUniform3f(FogPos, fog.x, fog.y, fog.z);

	GLint FogminPos = glGetUniformLocation(lightProgram, "fogMin");
	glUniform1f(FogminPos, fogMin);

	GLint FogmaxPos = glGetUniformLocation(lightProgram, "fogMax");
	glUniform1f(FogmaxPos, fogMax);
}

void DrawScene::ImGuiUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("ImGui");

	if (ImGui::Combo("Change center obj", &obj, objFile, IM_ARRAYSIZE(objFile)))
	{
		if (centerObject != obj)
		{
			centerObject = obj;

			if (centerObject == 0)
			{
				delete myMesh;

				myMesh = new OBJMesh("../Common/models/bunny.obj");
			}

			if (centerObject == 1)
			{
				delete myMesh;

				myMesh = new OBJMesh("../Common/models/cube2.obj");
			}

			if (centerObject == 2)
			{
				delete myMesh;

				myMesh = new OBJMesh("../Common/models/sphere.obj");
			}

			myMesh->setupBuffer();
			normalBufferSetUp();
			normalFaceBufferSetUp();
			gBufferSetUp();

			isVertexNormal = false;
			isFaceNormal = false;
		}
	}

	if (ImGui::TreeNode("Display Normal"))
	{
		ImGui::Checkbox("Display Vertex Normal", &isVertexNormal);
		ImGui::Checkbox("Display Face Normal", &isFaceNormal);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Constant"))
	{
		ImGui::DragFloat("Attenuation_constant", &constant, 0.1f, 0.f, 2.f);
		ImGui::DragFloat("Attenuation_linear", &linear, 0.1f, 0.f, 2.f);
		ImGui::DragFloat("Attenuation_quadratic", &quadratic, 0.1f, 0.f, 2.f);

		ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&globalAmbient));
		ImGui::ColorEdit3("Emissive", reinterpret_cast<float*>(&emmissive));
		ImGui::ColorEdit3("Coefficient", reinterpret_cast<float*>(&coefficient));

		ImGui::ColorEdit3("FogColor", reinterpret_cast<float*>(&fog));
		ImGui::DragFloat("FogMax", &fogMax, 0.1f, 6, 50.f);
		ImGui::DragFloat("FogMin", &fogMin, 0.1f, 0, 5.f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light Control"))
	{
		ImGui::Combo("LightType", &lightTypeChange, lightTypes, IM_ARRAYSIZE(lightTypes));

		if (lightType[0] != lightTypeChange)
		{
			for (int i = 0; i < currLightNum; i++)
			{
				lightType[i] = lightTypeChange;
			}
		}

		ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambients[0]));
		ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuses[0]));
		ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&speculars[0]));

		ImGui::DragFloat("Falloff", &falloff[0], 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("Inner angle", &innerAngle[0], 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Outer angle", &outerAngle[0], 0.1f, 0.0f, 100.0f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Defferred rendering"))
	{
		ImGui::Checkbox("Depth copying", &depth);

		ImGui::Combo("Render Type", &renderTypes, renders, IM_ARRAYSIZE(renders));

		if (renderType != renderTypes)
		{
			renderType = renderTypes;
		}

		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}