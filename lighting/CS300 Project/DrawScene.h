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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>
#include <fstream>
#include <sstream>

#include "shader.hpp"
#include "OBJReader.h"
#include "Mesh.h"
#include "Texture.h"

class Mesh;

class DrawScene: public Scene
{
public:
    enum class mesh
    {
        Sphere,
        Obj,
        ObjN,
        Orbit,
        floor,
        lamp,
        nothing,
    };

    DrawScene() = default;
    DrawScene(int /*windowWidth*/, int /*windowHeight*/, DrawScene::mesh type, std::string path = "0", int types = 1, bool entity = 0);
    virtual ~DrawScene();

    int Init() override;
    void CleanUp() override;

    int Render() override;
    int postRender() override;

    void SetupBuffers();
    void matrixUpdate();

    void setscaleVector(glm::vec3 scaleV)
    {
        scaleVector = scaleV;
        matrixUpdate();
    }

    void setCenter(glm::vec3 mcentroid)
    {
        center = mcentroid;
        matrixUpdate();
    }

    void setColor(glm::vec4 newColor)
    {
        colorVector = newColor;
    }

    void setRotate(glm::vec3 newRotate)
    {
        rotate = newRotate;
        matrixUpdate();
    }

    glm::mat4 GetModelMatrix()
    {
        return modelMat;
    }

    glm::vec4 GetColor()
    {
        return colorVector;
    }
    
    glm::vec3 GetCenter()
    {
        return myMesh.getModelCentroid();
    }

    void DoNormal();

    void SetUpNormalBuffer();
    void SetUpNormalFaceBuffer();

    void SetShader(GLuint shaderID)
    {
        nowShader = shaderID;
    }

    //////////////////////////////
    //////////////////////////////
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::vec3 scaleVector = glm::vec3(0.1f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotate = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat angleOfRotation;

    glm::vec4 colorVector = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    glm::mat4 modelNormal = glm::mat4(1.0f);
    glm::vec3 centerModel = glm::vec3(0.0f);

    ////////////////////////////////
    // /////////////////////////////
    mesh meshType;
    std::string paths;
    float modelSize = 0.0f;

    ////////////////////////////////
    //////////////////////////////// 
    Mesh myMesh;
    OBJReader* objReader = new(OBJReader);


    // data members
    GLuint VertexArrayID;
    GLuint vbo;
    GLuint ebo;
    GLuint norm;
    GLuint uv;

    ///////////////////
    //////////normal
    bool vertexCheck;
    bool faceCheck;

    GLuint NormalVao;
    GLuint NormalfaceVao;
    
    /////////for lamp
    GLuint lightVAO;

    ////////////
    GLuint nowShader;

    ///////////
    Texture myTexture;
    int uvTypes;
    bool isEntity;
    
};