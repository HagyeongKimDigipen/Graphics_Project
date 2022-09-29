/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OBJReader.cpp
Purpose: load obj file
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 10/15/2021
End Header --------------------------------------------------------*/
#include <iostream>
#include <cstring>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <cfloat>
#include <chrono>
#include <set>
#include "OBJReader.h"

OBJReader::OBJReader()
{
    initData();
}

OBJReader::~OBJReader()
{
    initData();
}

void OBJReader::initData()
{
    _currentMesh = nullptr;
}

double OBJReader::ReadOBJFile(std::string filepath, Mesh *pMesh,
        OBJReader::ReadMethod r, GLboolean bFlipNormals, int type, bool entitys)
{
    int rFlag = -1;

    if( pMesh )
        _currentMesh = pMesh;
    else
        return rFlag;

//    clock_t  startTime, endTime;

    auto startTime = std::chrono::high_resolution_clock::now();

    switch( r )
    {
    case OBJReader::ReadMethod::LINE_BY_LINE:
            rFlag = ReadOBJFile_LineByLine( filepath );
            break;

        case OBJReader::ReadMethod::BLOCK_IO:
            rFlag = ReadOBJFile_BlockIO( filepath );
            break;

        default:
        std::cout << "Unknown value for OBJReader::ReadMethod in function ReadObjFile." << std::endl;
        std::cout << "Quitting ..." << std::endl;
        rFlag = -1;
        break;
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    double timeDuration = std::chrono::duration< double, std::milli >( endTime - startTime ).count();

    std::cout << "OBJ file read in "
              << timeDuration
              << "  milli seconds." << std::endl;

    if (type == 0)
    {
        uvTypes = Mesh::UVType::PLANAR_UV;
    }
    else if(type == 1)
    {
        uvTypes = Mesh::UVType::CYLINDRICAL_UV;
    }
    else if (type == 2)
    {
        uvTypes = Mesh::UVType::SPHERICAL_UV;
    }
    else if (type == 3)
    {
        uvTypes = Mesh::UVType::CUBE_MAPPED_UV;
    }

    _currentMesh->calcVertexNormals(bFlipNormals);
    _currentMesh->calcUVs(uvTypes, entitys);

    return timeDuration;
}

Mesh* OBJReader::getMesh()
{
    return _currentMesh;
}

int OBJReader::ReadOBJFile_LineByLine(std::string filepath)
{
    int rFlag = -1;
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    std::ifstream  inFile;
    inFile.open( filepath );

    if( inFile.bad() || inFile.eof() || inFile.fail() )
        return rFlag;

    while( !inFile.eof() )
    {
        char buffer[256] = "\0";
        inFile.getline(buffer, 256, '\n');

        // Use only for debugging purpose
//        std::cout << buffer << std::endl;
        ParseOBJRecord( buffer, min, max );
    }

    _currentMesh->boundingBox[0] = min;
    _currentMesh->boundingBox[1] = max;

    return rFlag;
}

// Read the OBJ file in blocks -- works for files smaller than 1GB
int OBJReader::ReadOBJFile_BlockIO( std::string filepath )
{
    int rFlag = -1;
    long int OneGBinBytes = 1024 * 1024 * 1024 * sizeof(char);

    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);


    // Check the file size, if > 1 GB, abort
    std::ifstream inFile( filepath, std::ifstream::in | std::ifstream::binary );

    if( inFile.bad() || inFile.eof() )
        return rFlag;

    char *fileContents = NULL;
    long int count=0;

    // get the file size
    inFile.seekg(0, std::ifstream::end);
    count = static_cast<long>(inFile.tellg());
    inFile.seekg(0, std::ifstream::beg);

    if( count <= 0 || count >= OneGBinBytes )
    {
        std::cout << " Error reading file " << filepath << std::endl;
        std::cout << "File size reported as : " << count << " bytes." << std::endl;
    }
    else if (count > 0)
    {
        const char *delims = "\n\r";
        fileContents = (char *)malloc(sizeof(char) * (static_cast<unsigned long long>(count)+1));
        inFile.read(fileContents,count);
        if (fileContents != nullptr)
        {
            fileContents[count] = '\0';
        }
        rFlag = 0;

        // Now parse the obj file
        char *currPtr = fileContents;
        char *token = nullptr;

        if (currPtr != nullptr)
        {
            token = strpbrk(currPtr, delims);
        }

        while( token != nullptr )
        {
            int numChars = static_cast<int>(token - currPtr);
            char ObjLine[256];
            strncpy_s( ObjLine, currPtr, numChars );
            ObjLine[numChars]='\0';

            ParseOBJRecord( ObjLine, min, max );

            currPtr = token + 1;
            token = strpbrk( currPtr, delims );
        }

        free(fileContents);

        _currentMesh->boundingBox[0] = min;
        _currentMesh->boundingBox[1] = max;
    }

    return rFlag;
}

void OBJReader::ParseOBJRecord( char *buffer, glm::vec3 &min, glm::vec3 &max )
{
    const char *delims = " \r\n\t";
    GLfloat x, y, z;
    char* context = nullptr;

    GLfloat   temp;
    GLuint    firstIndex, secondIndex, thirdIndex;

    char *token = strtok_s( buffer, delims, &context);

    // account for empty lines
    if( token == nullptr )
        return;

    switch( token[0] )
    {
        case 'v':
            // vertex coordinates
            if( token[1] == '\0' )
            {
                token = strtok_s( nullptr, delims , &context);
                temp = static_cast<GLfloat &&>(atof(token));
                if( min.x > temp )
                    min.x = temp;
                if( max.x <= temp )
                    max.x = temp;
                x = temp;

                token = strtok_s( nullptr, delims, &context);
                temp = static_cast<GLfloat &&>(atof(token));
                if( min.y > temp )
                    min.y = temp;
                if( max.y <= temp )
                    max.y = temp;
                y = temp;

                token = strtok_s( nullptr, delims, &context);
                temp = static_cast<GLfloat &&>(atof(token));
                if( min.z > temp )
                    min.z = temp;
                if( max.z <= temp )
                    max.z = temp;
                z = temp;

                _currentMesh->vertexBuffer.emplace_back(x, y, z);
            }
                // vertex normals
            else if( token[1] == 'n' )
            {
                glm::vec3 vNormal;

                token = strtok_s( nullptr, delims, &context);
                if( token == nullptr )
                    break;

                vNormal[0] = static_cast<GLfloat &&>(atof(token));

                token = strtok_s( nullptr, delims, &context);
                if( token == nullptr )
                    break;

                vNormal[1] = static_cast<GLfloat &&>(atof(token));

                token = strtok_s( nullptr, delims, &context);
                if( token == nullptr )
                    break;

                vNormal[2] = static_cast<GLfloat &&>(atof(token));

                _currentMesh->vertexNormals.push_back( glm::normalize(vNormal) );
            }

            break;

        case 'f':
            token = strtok_s( nullptr, delims, &context);
            if( token == nullptr )
                break;
            firstIndex = static_cast<unsigned int &&>(atoi(token) - 1);

            token = strtok_s(nullptr, delims, &context);
            if( token == nullptr )
                break;
            secondIndex = static_cast<unsigned int &&>(atoi(token) - 1);

            token = strtok_s(nullptr, delims, &context);
            if( token == nullptr )
                break;
            thirdIndex = static_cast<unsigned int &&>(atoi(token) - 1);

            // push back first triangle
            _currentMesh->vertexIndices.push_back( firstIndex );
            _currentMesh->vertexIndices.push_back( secondIndex );
            _currentMesh->vertexIndices.push_back( thirdIndex );

            token = strtok_s(nullptr, delims, &context);

            while( token != nullptr )
            {
                secondIndex = thirdIndex;
                thirdIndex = static_cast<unsigned int &&>(atoi(token) - 1);

                _currentMesh->vertexIndices.push_back( firstIndex );
                _currentMesh->vertexIndices.push_back( secondIndex );
                _currentMesh->vertexIndices.push_back( thirdIndex );

                token = strtok_s(nullptr, delims, &context);
            }

            break;

        case '#':
        default:
            break;
    }

    return;
}
