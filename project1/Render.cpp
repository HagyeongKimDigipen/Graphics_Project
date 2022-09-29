/*
*	hakyung.kim
*	Assigment 2
*	CS250
*	spring 2020
*/

#include "Render.h"

Render::Render(void)
{
    const char* fragment_shader =
        "#version 130\n\
     uniform vec3 color;\
     out vec4 frag_color;\
     void main(void) {\
       frag_color = vec4(color,1);\
     }";
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fragment_shader, 0);
    glCompileShader(fshader);

    const char* vertex_shader =
        "#version 130\n\
     attribute vec4 position;\
     void main(void) {\
       gl_Position = position;\
     }";
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vertex_shader, 0);
    glCompileShader(vshader);

    program = glCreateProgram();
    glAttachShader(program, fshader);
    glAttachShader(program, vshader);
    glLinkProgram(program);

    aposition = glGetAttribLocation(program, "position");
    ucolor = glGetUniformLocation(program, "color");

    glUniform3f(ucolor, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
}

Render::~Render(void)
{
    glUseProgram(0);
    for (int i = 0; i < 2; ++i) {
        GLuint shaders[2];
        GLsizei count;
        glGetAttachedShaders(program, 2, &count, shaders);
        for (int j = 0; j < count; ++j)
            glDeleteShader(shaders[j]);
        glDeleteProgram(program);
    }
}

void Render::ClearBuffers(const Vector& c)
{
	glClearColor(c.x, c.y, c.z, c.w);
    glClearDepth(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SetColor(const Vector& c)
{
	glColor4f(c.x, c.y, c.z, c.w);
}

void Render::DrawLine(const Hcoord& P, const Hcoord& Q)
{
    glBegin(GL_LINES);

    glVertex4f(P.x, P.y, P.z, P.w);
    glVertex4f(Q.x, Q.y, Q.z, Q.w);

    glEnd();
    glFinish();
}

void Render::FillTriangle(const Hcoord& P, const Hcoord& Q, const Hcoord& R)
{
    glBegin(GL_TRIANGLES);

    glVertex4f(P.x, P.y, P.z, P.w);
    glVertex4f(Q.x, Q.y, Q.z, Q.w);
    glVertex4f(R.x, R.y, R.z, R.w);

    glEnd();
    glFinish();
}
