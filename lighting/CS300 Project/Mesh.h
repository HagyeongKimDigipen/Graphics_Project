/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: header of Mesh.cpp (draw shape and calculate normal)
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 09/23/2021
End Header --------------------------------------------------------*/
#pragma once

#include <filesystem>
#include <vector>
#include <gl/glew.h>
#include <string>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Mesh
{
public:
    GLfloat* getVertexBuffer();
    GLfloat* getVertexNormals();
    GLfloat* getVertexUVs();

    GLfloat* getVertexNormalsForDisplay();

    unsigned int getVertexBufferSize();
    unsigned int getVertexCount();
    unsigned int getVertexNormalCount();

    // Get vertex index buffer
    GLuint* getIndexBuffer();
    unsigned int getIndexBufferSize();
    unsigned int getTriangleCount();

    glm::vec3   getModelScale();
    glm::vec3   getModelCentroid();
    glm::vec3   getCentroidVector(glm::vec3 vVertex);


    GLfloat& getNormalLength();
    void setNormalLength(GLfloat nLength);

    // initialize the data members
    void initData();

    // calculate vertex normals
    int calcVertexNormals(GLboolean bFlipNormals = false);

    // calculate the "display" normals
    void calcVertexNormalsForDisplay(GLboolean bFlipNormals = false);

    // calculate texture coordinates
    enum class UVType {
        PLANAR_UV = 0,
        CYLINDRICAL_UV,
        SPHERICAL_UV,
        CUBE_MAPPED_UV
    };

    int         calcUVs(Mesh::UVType uvType = Mesh::UVType::PLANAR_UV, bool entity = 0);
    glm::vec2   calcCubeMap(glm::vec3 vEntity);

//private:
    std::vector<glm::vec3>    vertexBuffer;
    std::vector<GLuint>       vertexIndices;
    std::vector<glm::vec2>    vertexUVs;
    std::vector<glm::vec3>    vertexNormals, vertexNormalDisplay, faceNormalsDisplay;

    glm::vec3               boundingBox[2];
    GLfloat                 normalLength = 0.f;

    bool entityNormal;
};
