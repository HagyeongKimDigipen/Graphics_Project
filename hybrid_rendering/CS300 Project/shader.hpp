/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: shader.hpp
Purpose: header of shader.hpp (load shader file)
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 09/23/2021
End Header --------------------------------------------------------*/
#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader
{
public:
    enum Primitive_Enum
    {
        TriangleStrip = 0,
        Points,
        LineStrip
    };

    Shader();
    ~Shader();

    void noShader();
    void shaderUse(GLuint programID);

    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

    GLuint LoadPipeline(const char* vertex_file_path, const char* fragment_file_path, GLuint* programIDs);

    GLuint LoadMultiShaders(const char* vertex_file_path, const char* fragment_file_path,
        const char* geom_file_path, Primitive_Enum  out_primitive_type);

    GLuint ReloadShaders( GLuint* program, const char* vertex_shader_filename, const char* fragment_shader_filename);
};

#endif