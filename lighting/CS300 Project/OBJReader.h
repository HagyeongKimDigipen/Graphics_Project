/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OBJReader.h
Purpose: load obj file
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 10/15/2021
End Header --------------------------------------------------------*/

#ifndef SIMPLE_SCENE_OBJREADER_H
#define SIMPLE_SCENE_OBJREADER_H

#include <string>
#include <fstream>
#include <vector>

// for OpenGL datatypes
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"

class OBJReader
{

public:
    OBJReader();
    virtual ~OBJReader();

    // initialize the data
    void initData();


    // Read data from a file
    enum class ReadMethod { LINE_BY_LINE, BLOCK_IO };
    double ReadOBJFile(std::string filepath,
                       Mesh *pMesh,
                       ReadMethod r = ReadMethod::LINE_BY_LINE,
                       GLboolean bFlipNormals = false, int type = 1, bool entitys = 0);
    Mesh* getMesh();
private:

    // Read OBJ file line by line
    int ReadOBJFile_LineByLine( std::string filepath );

    // Read the OBJ file in blocks -- works for files smaller than 1GB
    int ReadOBJFile_BlockIO( std::string filepath );

    // Parse individual OBJ record (one line delimited by '\n')
    void ParseOBJRecord( char *buffer, glm::vec3 &min, glm::vec3 &max );

    // data members
    Mesh *      _currentMesh;
    Mesh::UVType uvTypes;
    bool entity = false;
};


#endif //SIMPLE_SCENE_OBJREADER_H
