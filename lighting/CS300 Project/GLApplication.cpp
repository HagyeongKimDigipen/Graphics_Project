/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GLApplication.cpp
Purpose: make window and draw
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS300 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221)
Creation date: 09/23/2021
End Header --------------------------------------------------------*/

#include <Windows.h>
#include <cstdio>
#include <vector>
#include <array>

// Include GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "shader.hpp"
#include  "DrawScene.h"

#include "imGui/imgui.h"
#include "imGui/imgui_impl_opengl3.h"
#include "imGui/imgui_impl_glfw.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////

GLFWwindow* window;

int windowWidth = 1024;
int windowHeight = 800;

///////for shader
GLuint programID = 0;
GLuint programID2 = 0;

Shader mShader;
Shader mShader2;

GLint vertexCamera = 0;
GLint vertexProjection = 0;
GLint vertexTransform = 0;
GLint lightPos = 0;
GLint lgihtColor = 0;
GLint viewPos = 0;
GLint vertexColor = 0;
GLint cameraPos = 0;

GLuint ubo;

glm::vec3 cameraPosition = glm::vec3(0.f, 1.5f, 5.0f);

///////////
float PI = 3.141592f;

glm::vec3 orbit1 = { 15.f * std::cos(PI / 8.f * 0), 0.f, 15.f * std::sin(PI / 8.f * 0) };
glm::vec3 orbit2 = { 15.f * std::cos(PI / 8.f * 1), 0.f, 15.f * std::sin(PI / 8.f * 1) };
glm::vec3 orbit3 = { 15.f * std::cos(PI / 8.f * 2), 0.f, 15.f * std::sin(PI / 8.f * 2) };
glm::vec3 orbit4 = { 15.f * std::cos(PI / 8.f * 3), 0.f, 15.f * std::sin(PI / 8.f * 3) };
glm::vec3 orbit5 = { 15.f * std::cos(PI / 8.f * 4), 0.f, 15.f * std::sin(PI / 8.f * 4) };
glm::vec3 orbit6 = { 15.f * std::cos(PI / 8.f * 5), 0.f, 15.f * std::sin(PI / 8.f * 5) };
glm::vec3 orbit7 = { 15.f * std::cos(PI / 8.f * 6), 0.f, 15.f * std::sin(PI / 8.f * 6) };
glm::vec3 orbit8 = { 15.f * std::cos(PI / 8.f * 7), 0.f, 15.f * std::sin(PI / 8.f * 7) };
glm::vec3 orbit9 = { 15.f * std::cos(PI / 8.f * 8), 0.f, 15.f * std::sin(PI / 8.f * 8) };
glm::vec3 orbit10 = { 15.f * std::cos(PI / 8.f * 9), 0.f, 15.f * std::sin(PI / 8.f * 9) };
glm::vec3 orbit11 = { 15.f * std::cos(PI / 8.f * 10), 0.f, 15.f * std::sin(PI / 8.f * 10) };
glm::vec3 orbit12 = { 15.f * std::cos(PI / 8.f * 11), 0.f, 15.f * std::sin(PI / 8.f * 11) };
glm::vec3 orbit13 = { 15.f * std::cos(PI / 8.f * 12), 0.f, 15.f * std::sin(PI / 8.f * 12) };
glm::vec3 orbit14 = { 15.f * std::cos(PI / 8.f * 13), 0.f, 15.f * std::sin(PI / 8.f * 13) };
glm::vec3 orbit15 = { 15.f * std::cos(PI / 8.f * 14), 0.f, 15.f * std::sin(PI / 8.f * 14) };
glm::vec3 orbit16 = { 15.f * std::cos(PI / 8.f * 15), 0.f, 15.f * std::sin(PI / 8.f * 15) };

glm::vec3 lightPoss = orbit8;

glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

//for uniform block && shader light
GLuint uniformLight;

glm::vec3 lightPosition[16];
glm::vec3 directionUniform[16];
glm::vec3 ambientUniform[16] = { {0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f},{0.05f, 0.05f, 0.05f} };
glm::vec3 diffuseUniform[16] = { {0.8f, 0.6f, 0.6f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f} ,{0.8f, 0.6f, 0.6f } ,{0.5f, 0.5f, 0.3f} ,{0.8f, 0.6f, 0.6f } ,{0.1f, 0.7f, 0.7f},{0.8f, 0.6f, 0.6f },{0.1f, 0.7f, 0.7f} };
glm::vec3 specularUniform[16] = { {0.2f, 0.2f, 0.2f},{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} ,{0.2f, 0.2f, 0.2f} };

GLfloat constantUniform = 1.0f;
GLfloat linearUniform = 0.1f;;
GLfloat quadraticUniform = 0.03f;

GLfloat falloffUniform[16] = { 1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };

GLfloat outerAngleUniform[16] = { 20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f,20.f };

GLfloat innerAngleUniform[16] = { 10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f,10.f };

GLint lightType[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

GLint curLightNum;
GLint curLightType;

int lightNum = 16;
int lightTypes = 0;

///////
GLint textureShader;
GLint textureShader2;

//material
GLint mAmbient;
GLint mDiffuse;
GLint mSpecular;
GLint mShininess;

///////draw mesh
DrawScene* orbit;
DrawScene* centerObj;
DrawScene* myfloor;

//draw lamp
DrawScene* myLamp;
DrawScene* myLamp2;
DrawScene* myLamp3;
DrawScene* myLamp4;
DrawScene* myLamp5;
DrawScene* myLamp6;
DrawScene* myLamp7;
DrawScene* myLamp8;
DrawScene* myLamp9;
DrawScene* myLamp10;
DrawScene* myLamp11;
DrawScene* myLamp12;
DrawScene* myLamp13;
DrawScene* myLamp14;
DrawScene* myLamp15;
DrawScene* myLamp16;

////for ImGui
bool checkUV = false;
bool GPUcalculation = false;
bool checkEntity = false;
bool checkDir = false;
bool checkPoint = false;
bool checkSpot = false;
bool noCenterObj = false;

glm::vec3 emmisive = { 0.f,0.f,0.f };
glm::vec3 coefficient = { 0.f,0.f,0.f };
glm::vec3 gAmbient = { 0.f,0.f,0.f };

glm::vec3 uvMin = { 0.f,0.f,0.f };
glm::vec3 uvMax = { 0.f,0.f,0.f };

glm::vec3 fog = { 0.5f,0.5f,0.5f };
float fogMin = 2.f;
float fogMax = 30.f;

/////// control global
GLint checkUVs;
GLint GPUcalculations;
GLint checkEntitys;
GLint checkDirs;
GLint checkPoints;
GLint checkSpots;

GLint emmisives;
GLint coefficients;
GLint gAmbients;
GLint uvMins;
GLint uvMaxs;

GLint fogs;
GLint fogMins;
GLint fogMaxs;

////////// texture
bool isTexture = true;
GLint isTextures;
int uvTypeCheck = 1;
GLint uvTypeChecks;

//////////function
glm::vec3 colorChange(glm::vec3 color_255);
void initMesh(int uvType, bool entity = 0);
void setShader();
void lampSetUP();
void setLightShader(int index = 0);

int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        //getchar();
        return -1;
    }

    // Setting up OpenGL properties
    glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, // window dimensions
        "CS300_Assignment2", // window title
        nullptr, // which monitor (if full-screen mode)
        nullptr); // if sharing context with another window
    if (window == nullptr)
    {
        fprintf(stderr, "Failed to open GLFW window. Check if your GPU is 4.0 compatible.\n");
        //getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        //getchar();
        glfwTerminate();
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    programID = mShader.LoadShaders("../Common/shaders/CS300ShaderPhongShading.vert", "../Common/shaders/CS300ShaderPhongShading.frag");
    //programID = mShader.LoadShaders("../Common/shaders/CS300ShaderPhongLighting.vert", "../Common/shaders/CS300ShaderPhongLighting.frag");
    //programID = mShader.LoadShaders("../Common/shaders/CS300ShaderBlinnShading.vert", "../Common/shaders/CS300ShaderBlinnShading.frag");

    programID2 = mShader2.LoadShaders("../Common/shaders/LampShader.vert", "../Common/shaders/LampShader.frag");
    
    mShader2.shaderUse(programID2);
    mShader.shaderUse(programID);

    //set shader uniform value
    setShader();
    setLightShader();

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Initialize the scene
    initMesh(uvTypeCheck);
    lampSetUP();

    glBindVertexArray(0);

    bool textureCheck1 = false;
    bool textureCheck2 = false;
    bool textureCheck3 = false;
    bool textureCheck4 = false;

    bool calculating = false;
    bool calculating2 = false;

    bool entityCheck = false;
    bool entityCheck2 = false;

    bool checkVertexNormal{ false };
    bool checkFaceormal{ false };

    bool shaderCheck = false;
    bool shaderCheck2 = false;
    bool shaderCheck3 = false;

    int obj{ 0 };

    int lightT{ 1 };
    int lightT2{ 1 };
    int lightT3{ 1 };
    int lightT4{ 1 };
    int lightT5{ 1 };
    int lightT6{ 1 };
    int lightT7{ 1 };
    int lightT8{ 1 };
    int lightT9{ 1 };
    int lightT10{ 1 };
    int lightT11{ 1 };
    int lightT12{ 1 };
    int lightT13{ 1 };
    int lightT14{ 1 };
    int lightT15{ 1 };
    int lightT16{ 1 };

    int sinario{ 0 };
    int matrials{ 1 };
    int calculations = { 0 };
    int entitiys = { 0 };
    int shaders = { 0 };

    bool continueGPU = false;

    std::array objectFile = {
    "../Common/models/bunny.obj",
    "../Common/models/cube.obj",
    "../Common/models/cube2.obj",
    "../Common/models/sphere.obj",
    "../Common/models/sphere_modified.obj",
    "../Common/models/4Sphere.obj",
    "../Common/models/bunny_high_poly.obj",
    "../Common/models/cup.obj",
    "../Common/models/lucy_princeton.obj",
    "../Common/models/quad.obj",
    "../Common/models/starwars1.obj",
    "../Common/models/rhino.obj",
    "../Common/models/triangle.obj",
    };

    std::array lightsType = {
        "direction",
        "point",
        "spot"
    };

    std::array scenarios = {
    "none",
    "sinario 1",
    "sinario 2",
    "sinario 3"
    };

    std::array matrial = {
        "None",
        "Cylindrical",
        "Spherical",
        "Cube"
    };

    std::array calculation = {
        "CPU",
        "GPU"
    };

    std::array entity = {
        "Positon",
        "Normal"
    };

    std::array shader = {
        "Phong Shading",
        "Phong Lighing",
        "Blinn Shading"
    };

    /////// personal color
    glm::vec3 plum = colorChange({ 221.f,160.f,221.f });
    glm::vec3 lightcyan = colorChange({ 224.f,255.f,255.f });
    glm::vec3 lavenderblush = colorChange({ 255.f,240.f,245.f });
    glm::vec3 darkslateblue = colorChange({ 72.f,61.f,0139.f });
    glm::vec3 mediumpruple = colorChange({ 147.f,112.f,219.f });
    glm::vec3 yellow = colorChange({ 255.f,255.f,0.f });
    glm::vec3 mediumturquoise = colorChange({ 72.f,209.f,204.f });
    glm::vec3 aquamarine = colorChange({ 127.f,255.f,212.f });
    glm::vec3 lightpink = colorChange({ 255.f,182.f,193.f });
    glm::vec3 palevioletred = colorChange({ 219.f,112.f,147.f });
    glm::vec3 turquoise = colorChange({ 64.f,224.f,208.f });
    glm::vec3 thistle = colorChange({ 216.f,191.f,216.f });
    glm::vec3 red = colorChange({ 255.f,0.f,0.f });
    glm::vec3 darkkhaki = colorChange({ 189.f,183.f,107.f });
    glm::vec3 silver = colorChange({ 192.f,192.f,192.f });
    glm::vec3 olive = colorChange({ 128.f,128.f,0.f });
    /////////////////////////

    do
    {
        glClearColor(fog.x, fog.y, fog.z, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("ImGui");

            ImGui::Text("In GPU calculation mode, shader is fixed to phong shading,");

            if (ImGui::Combo("Change center obj", &obj, objectFile.data(), static_cast<int>(objectFile.size())))
            {
                delete centerObj;
                centerObj = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::Obj, objectFile[obj], uvTypeCheck);
                centerObj->Init();
                centerObj->setscaleVector(glm::vec3(0.5f));
                centerObj->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
                centerObj->setCenter(glm::vec3(0.0f, 0.0f, 0.0f));
            }

            if (ImGui::TreeNode("shader & normal"))
            {
                ImGui::Checkbox("vertex normals", &checkVertexNormal);

                if (checkVertexNormal)
                {
                    centerObj->vertexCheck = true;
                }
                else
                {
                    centerObj->vertexCheck = false;
                }

                ImGui::Checkbox("face normals", &checkFaceormal);

                if (checkFaceormal)
                {
                    centerObj->faceCheck = true;
                }
                else
                {
                    centerObj->faceCheck = false;
                }

                if (ImGui::Combo("shader reload", &shaders, shader.data(), static_cast<int>(shader.size())))
                {
                    if (continueGPU == false)
                    {
                        if (shaders == 0)
                        {
                            if (shaderCheck == false)
                            {
                                programID = mShader.ReloadShaders(&programID, "../Common/shaders/CS300ShaderPhongShading.vert", "../Common/shaders/CS300ShaderPhongShading.frag");
                                initMesh(uvTypeCheck);
                                setShader();
                                setLightShader();

                                shaderCheck = true;
                                shaderCheck2 = false;
                                shaderCheck3 = false;
                            }
                        }
                        else if (shaders == 1)
                        {
                            if (shaderCheck2 == false)
                            {
                                programID = mShader.ReloadShaders(&programID, "../Common/shaders/CS300ShaderPhongLighting.vert", "../Common/shaders/CS300ShaderPhongLighting.frag");
                                shaderCheck = true;
                                initMesh(uvTypeCheck);
                                setShader();
                                setLightShader();

                                shaderCheck = false;
                                shaderCheck2 = true;
                                shaderCheck3 = false;
                            }
                        }
                        else if (shaders == 2)
                        {
                            if (shaderCheck3 == false)
                            {
                                programID = mShader.ReloadShaders(&programID, "../Common/shaders/CS300ShaderBlinnShading.vert", "../Common/shaders/CS300ShaderBlinnShading.frag");
                                shaderCheck2 = true;
                                initMesh(uvTypeCheck);
                                setShader();
                                setLightShader();

                                shaderCheck = false;
                                shaderCheck2 = false;
                                shaderCheck3 = true;
                            }
                        }
                    }
                    else
                    {
                        shaders = 0;
                    }

                }

                ImGui::TreePop();
            }
           
            if (ImGui::TreeNode("constant"))
            {
                ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&gAmbient));
                ImGui::ColorEdit3("Emissive", reinterpret_cast<float*>(&emmisive));
                ImGui::ColorEdit3("Coefficient", reinterpret_cast<float*>(&coefficient));

                ImGui::DragFloat("Attenuation_constant", &constantUniform, 0.1f, 0.f, 2.f);
                ImGui::DragFloat("Attenuation_linear", &linearUniform, 0.1f, 0.f, 2.f);
                ImGui::DragFloat("Attenuation_quadratic", &quadraticUniform, 0.1f, 0.f, 2.f);

                ImGui::ColorEdit3("FogColor", reinterpret_cast<float*>(&fog));
                ImGui::DragFloat("FogMax", &fogMax, 0.1f, 6, 50.f);
                ImGui::DragFloat("FogMin", &fogMin, 0.1f, 0, 5.f);

                ImGui::TreePop();
            }
           

            if (ImGui::TreeNode("matrial"))
            {
                if (ImGui::Combo("Texture Projection Pipeline", &calculations, calculation.data(), static_cast<int>(calculation.size())))
                {
                    if (calculations == 0)
                    {
                        if (calculating == false)
                        {
                            programID = mShader.ReloadShaders(&programID, "../Common/shaders/CS300ShaderPhongShading.vert", "../Common/shaders/CS300ShaderPhongShading.frag");

                            initMesh(uvTypeCheck);
                            setShader();
                            setLightShader();

                            calculating = true;
                            calculating2 = false;

                            continueGPU = false;
                            entitiys = 0;
                        }
                    }
                    else if (calculations == 1)
                    {
                        if (calculating2 == false)
                        {
                            programID = mShader.ReloadShaders(&programID, "../Common/shaders/CS300ShaderPhongShading.vert", "../Common/shaders/CS300ShaderPhongShadingGPU.frag");

                            initMesh(uvTypeCheck);
                            setShader();
                            setLightShader();

                            calculating2 = true;
                            calculating = false;

                            continueGPU = true;
                            entitiys = 0;
                        }
                    }
                }

                if (ImGui::Combo("Texture Projection Mode", &matrials, matrial.data(), static_cast<int>(matrial.size())))
                {
                     if (matrials == 0)
                     {
                         if (textureCheck1 == false)
                         {
                             if (matrials != uvTypeCheck)
                             {
                                 uvTypeCheck = matrials;
                                 initMesh(uvTypeCheck);
                                 entitiys = 0;
                             }

                             textureCheck1 = true;
                             textureCheck2 = false;
                             textureCheck3 = false;
                             textureCheck4 = false;
                         }
                     }
                     else if (matrials == 1)
                     {
                         if (textureCheck2 == false)
                         {
                             if (matrials != uvTypeCheck)
                             {
                                 uvTypeCheck = matrials;
                                 initMesh(uvTypeCheck);
                                 entitiys = 0;
                             }

                             textureCheck1 = false;
                             textureCheck2 = true;
                             textureCheck3 = false;
                             textureCheck4 = false;
                         }
                     }
                     else if (matrials == 2)
                     {
                         if (textureCheck3 == false)
                         {
                             if (matrials != uvTypeCheck)
                             {
                                 uvTypeCheck = matrials;
                                 initMesh(uvTypeCheck);
                                 entitiys = 0;
                             }

                             textureCheck1 = false;
                             textureCheck2 = false;
                             textureCheck3 = true;
                             textureCheck4 = false;
                         }
                     }
                     else if (matrials == 3)
                     {
                         if (textureCheck4 == false)
                         {
                             if (matrials != uvTypeCheck)
                             {
                                 uvTypeCheck = matrials;
                                 initMesh(uvTypeCheck);
                                 entitiys = 0;
                             }

                             textureCheck1 = false;
                             textureCheck2 = false;
                             textureCheck3 = false;
                             textureCheck4 = true;
                         }
                     }
                    
                }

                if (ImGui::Combo("Texture Entity", &entitiys, entity.data(), static_cast<int>(entity.size())))
                {                   
                    if (entitiys == 0)
                    {
                        if (entityCheck == false)
                        {
                            checkEntity = entitiys;

                            entityCheck = true;
                            entityCheck2 = false;

                            initMesh(uvTypeCheck, 0);
                        }
                    }
                    else if (entitiys == 1)
                    {
                        if (entityCheck2 == false)
                        {
                            checkEntity = entitiys;

                            entityCheck2 = true;
                            entityCheck = false;

                            initMesh(uvTypeCheck, 1);
                        }
                    }
                    
                }

                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("scenario"))
            {
                if (ImGui::Combo("scenario", &sinario, scenarios.data(), static_cast<int>(scenarios.size())))
                {
                    if (sinario == 1)
                    {
                        lightT = 1;
                        lightT2 = 1;
                        lightT3 = 1;
                        lightT4 = 1;
                        lightT5 = 1;
                        lightT6 = 1;
                        lightT7 = 1;
                        lightT8 = 1;
                        lightT9 = 1;
                        lightT10 = 1;
                        lightT11 = 1;
                        lightT12 = 1;
                        lightT13 = 1;
                        lightT14 = 1;
                        lightT15 = 1;
                        lightT16 = 1;

                        lightType[0] = 1;
                        lightType[1] = 1;
                        lightType[2] = 1;
                        lightType[3] = 1;
                        lightType[4] = 1;
                        lightType[5] = 1;
                        lightType[6] = 1;
                        lightType[7] = 1;
                        lightType[8] = 1;
                        lightType[9] = 1;
                        lightType[10] = 1;
                        lightType[11] = 1;
                        lightType[12] = 1;
                        lightType[13] = 1;
                        lightType[14] = 1;
                        lightType[15] = 1;

                        diffuseUniform[0] = { mediumpruple };
                        diffuseUniform[1] = { mediumpruple };
                        diffuseUniform[2] = { mediumpruple };
                        diffuseUniform[3] = { mediumpruple };
                        diffuseUniform[4] = { mediumpruple };
                        diffuseUniform[5] = { mediumpruple };
                        diffuseUniform[6] = { mediumpruple };
                        diffuseUniform[7] = { mediumpruple };
                        diffuseUniform[8] = { mediumpruple };
                        diffuseUniform[9] = { mediumpruple };
                        diffuseUniform[10] = { mediumpruple };
                        diffuseUniform[11] = { mediumpruple };
                        diffuseUniform[12] = { mediumpruple };
                        diffuseUniform[13] = { mediumpruple };
                        diffuseUniform[14] = { mediumpruple };
                        diffuseUniform[15] = { mediumpruple };
                    }
                    else if (sinario == 2)
                    {
                        lightT = 2;
                        lightT2 = 2;
                        lightT3 = 2;
                        lightT4 = 2;
                        lightT5 = 2;
                        lightT6 = 2;
                        lightT7 = 2;
                        lightT8 = 2;
                        lightT9 = 2;
                        lightT10 = 2;
                        lightT11 = 2;
                        lightT12 = 2;
                        lightT13 = 2;
                        lightT14 = 2;
                        lightT15 = 2;
                        lightT16 = 2;

                        lightType[0] = 2;
                        lightType[1] = 2;
                        lightType[2] = 2;
                        lightType[3] = 2;
                        lightType[4] = 2;
                        lightType[5] = 2;
                        lightType[6] = 2;
                        lightType[7] = 2;
                        lightType[8] = 2;
                        lightType[9] = 2;
                        lightType[10] = 2;
                        lightType[11] = 2;
                        lightType[12] = 2;
                        lightType[13] = 2;
                        lightType[14] = 2;
                        lightType[15] = 2;

                        diffuseUniform[0] = { plum };
                        diffuseUniform[1] = { lightcyan };
                        diffuseUniform[2] = { lavenderblush };
                        diffuseUniform[3] = { darkslateblue };
                        diffuseUniform[4] = { mediumpruple };
                        diffuseUniform[5] = { yellow };
                        diffuseUniform[6] = { mediumturquoise };
                        diffuseUniform[7] = { aquamarine };
                        diffuseUniform[8] = { lightpink };
                        diffuseUniform[9] = { palevioletred };
                        diffuseUniform[10] = { turquoise };
                        diffuseUniform[11] = { thistle };
                        diffuseUniform[12] = { red };
                        diffuseUniform[13] = { darkkhaki };
                        diffuseUniform[14] = { silver };
                        diffuseUniform[15] = { olive };
                    }
                    else if (sinario == 3)
                    {
                        lightT = 0;
                        lightT2 = 2;
                        lightT3 = 1;
                        lightT4 = 0;
                        lightT5 = 2;
                        lightT6 = 1;
                        lightT7 = 0;
                        lightT8 = 2;
                        lightT9 = 1;
                        lightT10 = 0;
                        lightT11 = 2;
                        lightT12 = 1;
                        lightT13 = 0;
                        lightT14 = 2;
                        lightT15 = 1;
                        lightT16 = 0;


                        lightType[0] = 0;
                        lightType[1] = 2;
                        lightType[2] = 1;
                        lightType[3] = 0;
                        lightType[4] = 2;
                        lightType[5] = 1;
                        lightType[6] = 0;
                        lightType[7] = 2;
                        lightType[8] = 1;
                        lightType[9] = 0;
                        lightType[10] = 2;
                        lightType[11] = 1;
                        lightType[12] = 0;
                        lightType[13] = 2;
                        lightType[14] = 1;
                        lightType[15] = 0;

                        diffuseUniform[0] = { plum };
                        diffuseUniform[1] = { lightcyan };
                        diffuseUniform[2] = { lavenderblush };
                        diffuseUniform[3] = { darkslateblue };
                        diffuseUniform[4] = { mediumpruple };
                        diffuseUniform[5] = { yellow };
                        diffuseUniform[6] = { mediumturquoise };
                        diffuseUniform[7] = { aquamarine };
                        diffuseUniform[8] = { lightpink };
                        diffuseUniform[9] = { palevioletred };
                        diffuseUniform[10] = { turquoise };
                        diffuseUniform[11] = { thistle };
                        diffuseUniform[12] = { red };
                        diffuseUniform[13] = { darkkhaki };
                        diffuseUniform[14] = { silver };
                        diffuseUniform[15] = { olive };
                    }
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Light Source"))
            {
                ImGui::DragInt("Light Count", &lightNum, 1, 0, 16);
                glUniform1i(curLightNum, lightNum);

                if (ImGui::TreeNode("Light Control"))
                {
                    if (lightNum >= 1)
                    {
                        if (ImGui::TreeNode("Light 1"))
                        {
                            ImGui::Combo("Light Type", &lightT, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[0] = lightT;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[0]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[0]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[0]));

                            ImGui::DragFloat("Falloff", &falloffUniform[0], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[0], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[0], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 2)
                    {
                        if (ImGui::TreeNode("Light 2"))
                        {
                            ImGui::Combo("Light Type", &lightT2, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[1] = lightT2;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[1]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[1]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[1]));

                            ImGui::DragFloat("Falloff", &falloffUniform[1], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[1], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[1], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 3)
                    {
                        if (ImGui::TreeNode("Light 3"))
                        {
                            ImGui::Combo("Light Type", &lightT3, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[2] = lightT3;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[2]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[2]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[2]));

                            ImGui::DragFloat("Falloff", &falloffUniform[2], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[2], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[2], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }

                    if (lightNum >= 4)
                    {
                        if (ImGui::TreeNode("Light 4"))
                        {
                            ImGui::Combo("Light Type", &lightT4, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[3] = lightT4;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[3]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[3]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[3]));

                            ImGui::DragFloat("Falloff", &falloffUniform[3], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[3], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[3], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 5)
                    {
                        if (ImGui::TreeNode("Light 5"))
                        {
                            ImGui::Combo("Light Type", &lightT5, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[4] = lightT5;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[4]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[4]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[4]));

                            ImGui::DragFloat("Falloff", &falloffUniform[4], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[4], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[4], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 6)
                    {
                        if (ImGui::TreeNode("Light 6"))
                        {
                            ImGui::Combo("Light Type", &lightT6, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[5] = lightT6;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[5]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[5]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[5]));

                            ImGui::DragFloat("Falloff", &falloffUniform[5], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[5], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[5], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 7)
                    {
                        if (ImGui::TreeNode("Light 7"))
                        {
                            ImGui::Combo("Light Type", &lightT7, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[6] = lightT7;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[6]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[6]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[6]));

                            ImGui::DragFloat("Falloff", &falloffUniform[6], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[6], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[6], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 8)
                    {
                        if (ImGui::TreeNode("Light 8"))
                        {
                            ImGui::Combo("Light Type", &lightT8, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[7] = lightT8;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[7]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[7]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[7]));

                            ImGui::DragFloat("Falloff", &falloffUniform[7], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[7], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[7], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 9)
                    {
                        if (ImGui::TreeNode("Light 9"))
                        {
                            ImGui::Combo("Light Type", &lightT9, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[8] = lightT9;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[8]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[8]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[8]));

                            ImGui::DragFloat("Falloff", &falloffUniform[8], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[8], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[8], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 10)
                    {
                        if (ImGui::TreeNode("Light 10"))
                        {
                            ImGui::Combo("Light Type", &lightT10, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[9] = lightT10;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[9]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[9]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[9]));

                            ImGui::DragFloat("Falloff", &falloffUniform[9], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[9], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[9], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 11)
                    {
                        if (ImGui::TreeNode("Light 11"))
                        {
                            ImGui::Combo("Light Type", &lightT11, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[10] = lightT11;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[10]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[10]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[10]));

                            ImGui::DragFloat("Falloff", &falloffUniform[10], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[10], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[10], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 12)
                    {
                        if (ImGui::TreeNode("Light 12"))
                        {
                            ImGui::Combo("Light Type", &lightT12, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[11] = lightT12;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[11]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[11]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[11]));

                            ImGui::DragFloat("Falloff", &falloffUniform[11], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[11], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[11], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 13)
                    {
                        if (ImGui::TreeNode("Light 13"))
                        {
                            ImGui::Combo("Light Type", &lightT13, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[12] = lightT13;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[12]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[12]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[12]));

                            ImGui::DragFloat("Falloff", &falloffUniform[12], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[12], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[2], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 14)
                    {
                        if (ImGui::TreeNode("Light 14"))
                        {
                            ImGui::Combo("Light Type", &lightT14, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[13] = lightT14;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[13]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[13]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[13]));

                            ImGui::DragFloat("Falloff", &falloffUniform[13], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[13], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[13], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 15)
                    {
                        if (ImGui::TreeNode("Light 15"))
                        {
                            ImGui::Combo("Light Type", &lightT15, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[14] = lightT15;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[14]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[14]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[14]));

                            ImGui::DragFloat("Falloff", &falloffUniform[14], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[14], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[14], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    if (lightNum >= 16)
                    {
                        if (ImGui::TreeNode("Light 16"))
                        {
                            ImGui::Combo("Light Type", &lightT, lightsType.data(), static_cast<int>(lightsType.size()));
                            lightType[15] = lightT16;

                            ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&ambientUniform[15]));
                            ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&diffuseUniform[15]));
                            ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&specularUniform[15]));

                            ImGui::DragFloat("Falloff", &falloffUniform[15], 0.01f, 0.0f, 10.0f);
                            ImGui::DragFloat("Inner angle", &innerAngleUniform[15], 0.1f, 0.0f, 100.0f);
                            ImGui::DragFloat("Outer angle", &outerAngleUniform[15], 0.1f, 0.0f, 100.0f);

                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }

            ImGui::End();
            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        lightPosition[0] = glm::vec3(myLamp->GetModelMatrix()[3].x, myLamp->GetModelMatrix()[3].y, myLamp->GetModelMatrix()[3].z);
        lightPosition[1] = glm::vec3(myLamp2->GetModelMatrix()[3].x, myLamp2->GetModelMatrix()[3].y, myLamp2->GetModelMatrix()[3].z);
        lightPosition[2] = glm::vec3(myLamp3->GetModelMatrix()[3].x, myLamp3->GetModelMatrix()[3].y, myLamp3->GetModelMatrix()[3].z);
        lightPosition[3] = glm::vec3(myLamp4->GetModelMatrix()[3].x, myLamp4->GetModelMatrix()[3].y, myLamp4->GetModelMatrix()[3].z);
        lightPosition[4] = glm::vec3(myLamp5->GetModelMatrix()[3].x, myLamp5->GetModelMatrix()[3].y, myLamp5->GetModelMatrix()[3].z);
        lightPosition[5] = glm::vec3(myLamp6->GetModelMatrix()[3].x, myLamp6->GetModelMatrix()[3].y, myLamp6->GetModelMatrix()[3].z);
        lightPosition[6] = glm::vec3(myLamp7->GetModelMatrix()[3].x, myLamp7->GetModelMatrix()[3].y, myLamp7->GetModelMatrix()[3].z);
        lightPosition[7] = glm::vec3(myLamp8->GetModelMatrix()[3].x, myLamp8->GetModelMatrix()[3].y, myLamp8->GetModelMatrix()[3].z);
        lightPosition[8] = glm::vec3(myLamp9->GetModelMatrix()[3].x, myLamp9->GetModelMatrix()[3].y, myLamp9->GetModelMatrix()[3].z);
        lightPosition[9] = glm::vec3(myLamp10->GetModelMatrix()[3].x, myLamp10->GetModelMatrix()[3].y, myLamp10->GetModelMatrix()[3].z);
        lightPosition[10] = glm::vec3(myLamp11->GetModelMatrix()[3].x, myLamp11->GetModelMatrix()[3].y, myLamp11->GetModelMatrix()[3].z);
        lightPosition[11] = glm::vec3(myLamp12->GetModelMatrix()[3].x, myLamp12->GetModelMatrix()[3].y, myLamp12->GetModelMatrix()[3].z);
        lightPosition[12] = glm::vec3(myLamp13->GetModelMatrix()[3].x, myLamp13->GetModelMatrix()[3].y, myLamp13->GetModelMatrix()[3].z);
        lightPosition[13] = glm::vec3(myLamp14->GetModelMatrix()[3].x, myLamp14->GetModelMatrix()[3].y, myLamp14->GetModelMatrix()[3].z);
        lightPosition[14] = glm::vec3(myLamp15->GetModelMatrix()[3].x, myLamp15->GetModelMatrix()[3].y, myLamp15->GetModelMatrix()[3].z);
        lightPosition[15] = glm::vec3(myLamp16->GetModelMatrix()[3].x, myLamp16->GetModelMatrix()[3].y, myLamp16->GetModelMatrix()[3].z);

        mShader.shaderUse(programID);
        setLightShader();
        setShader();

        glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &centerObj->GetModelMatrix()[0][0]);
        glUniform3f(vertexColor, lavenderblush.x, lavenderblush.y, lavenderblush.z);
        glUniform1i(isTextures, true);
        centerObj->Display();

        uvMin = centerObj->myMesh.boundingBox[0];
        uvMax = centerObj->myMesh.boundingBox[1];

        glUniform3f(uvMins, uvMin.x, uvMin.y, uvMin.z);
        glUniform3f(uvMaxs, uvMax.x, uvMax.y, uvMax.z);

        glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &((glm::mat4(1.0f))[0][0]));
        glUniform3f(vertexColor, 0.f, 0.f, 0.f);
        glUniform1i(isTextures, false);
        orbit->Display();

        glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myfloor->GetModelMatrix()[0][0]);
        glUniform3f(vertexColor, 0.f, 0.f, 0.f);
        glUniform1i(isTextures, false);
        myfloor->Display();

        mShader2.shaderUse(programID2);
        lampSetUP();

        if (lightNum >= 1)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[0].x, diffuseUniform[0].y, diffuseUniform[0].z);
            myLamp->Display();
        }

        if (lightNum >= 2)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp2->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[1].x, diffuseUniform[1].y, diffuseUniform[1].z);

            myLamp2->Display();
        }

        if (lightNum >= 3)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp3->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[2].x, diffuseUniform[2].y, diffuseUniform[2].z);

            myLamp3->Display();
        }

        if (lightNum >= 4)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp4->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[3].x, diffuseUniform[3].y, diffuseUniform[3].z);
            myLamp4->Display();
        }

        if (lightNum >= 5)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp5->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[4].x, diffuseUniform[4].y, diffuseUniform[4].z);
            myLamp5->Display();
        }

        if (lightNum >= 6)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp6->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[5].x, diffuseUniform[5].y, diffuseUniform[5].z);
            myLamp6->Display();
        }

        if (lightNum >= 7)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp7->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[6].x, diffuseUniform[6].y, diffuseUniform[6].z);
            myLamp7->Display();
        }

        if (lightNum >= 8)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp8->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[7].x, diffuseUniform[7].y, diffuseUniform[7].z);
            myLamp8->Display();
        }

        if (lightNum >= 9)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp9->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[8].x, diffuseUniform[8].y, diffuseUniform[8].z);
            myLamp9->Display();
        }

        if (lightNum >= 10)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp10->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[9].x, diffuseUniform[9].y, diffuseUniform[9].z);
            myLamp10->Display();
        }

        if (lightNum >= 11)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp11->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[10].x, diffuseUniform[10].y, diffuseUniform[10].z);
            myLamp11->Display();
        }

        if (lightNum >= 12)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp12->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[11].x, diffuseUniform[11].y, diffuseUniform[11].z);
            myLamp12->Display();
        }

        if (lightNum >= 13)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp13->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[12].x, diffuseUniform[12].y, diffuseUniform[12].z);
            myLamp13->Display();
        }

        if (lightNum >= 14)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp14->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[13].x, diffuseUniform[13].y, diffuseUniform[13].z);
            myLamp14->Display();
        }

        if (lightNum >= 15)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp15->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[14].x, diffuseUniform[14].y, diffuseUniform[14].z);
            myLamp15->Display();
        }

        if (lightNum >= 16)
        {
            glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &myLamp16->GetModelMatrix()[0][0]);
            glUniform3f(vertexColor, diffuseUniform[15].x, diffuseUniform[15].y, diffuseUniform[15].z);
            myLamp16->Display();
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

glm::vec3 colorChange(glm::vec3 color_255)
{
    glm::vec3 newColor = { 0.f,0.f,0.f };

    newColor.x = color_255.x / 255;
    newColor.y = color_255.y / 255;
    newColor.z = color_255.z / 255;

    return newColor;
}

void initMesh(int uvType, bool entity)
{
    glUseProgram(programID);

    orbit = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::Orbit);
    centerObj = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::Obj, "../Common/models/bunny.obj", uvType, entity);

    myfloor = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::floor);

    orbit->Init();
    orbit->setscaleVector(glm::vec3(0.5f));

    centerObj->Init();
    centerObj->setscaleVector(glm::vec3(0.5f));
    centerObj->setCenter(glm::vec3(0.0f, 0.0f, 0.0f));
    centerObj->SetShader(programID);
    //textureShader = centerObj->LoadTexture("../Common/textures/metal_roof_diff_512x512.png", programID);
    textureShader = centerObj->myTexture.LoadTexture("../Common/textures/metal_roof_diff_512x512.png", programID);
    //textureShader = centerObj->myTexture.ReadPPMFile("../Common/textures/metal_roof_diff_512x512.ppm");

    textureShader2 = centerObj->myTexture.LoadTexture("../Common/textures/metal_roof_spec_512x512.png", programID);
    //textureShader2 = centerObj->myTexture.ReadPPMFile("../Common/textures/metal_roof_spec_512x512.ppm");
    
    myfloor->Init();
    myfloor->setscaleVector(glm::vec3(5.0f));
    myfloor->setCenter(glm::vec3(0.0f, -0.5f, -1.2f));
    myfloor->myTexture.LoadTexture("../Common/textures/metal_roof_diff_512x512.png", programID);
    /////lamp
    myLamp = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp->Init();
    myLamp->setscaleVector(glm::vec3(0.1f));
    myLamp->setCenter(orbit1);

    myLamp2 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp2->Init();
    myLamp2->setscaleVector(glm::vec3(0.1f));
    myLamp2->setCenter(orbit2);

    myLamp3 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp3->Init();
    myLamp3->setscaleVector(glm::vec3(0.1f));
    myLamp3->setCenter(orbit3);

    myLamp4 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp4->Init();
    myLamp4->setscaleVector(glm::vec3(0.1f));
    myLamp4->setCenter(orbit4);

    myLamp5 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp5->Init();
    myLamp5->setscaleVector(glm::vec3(0.1f));
    myLamp5->setCenter(orbit5);

    myLamp6 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp6->Init();
    myLamp6->setscaleVector(glm::vec3(0.1f));
    myLamp6->setCenter(orbit6);

    myLamp7 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp7->Init();
    myLamp7->setscaleVector(glm::vec3(0.1f));
    myLamp7->setCenter(orbit7);

    myLamp8 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp8->Init();
    myLamp8->setscaleVector(glm::vec3(0.1f));
    myLamp8->setCenter(orbit8);

    myLamp9 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp9->Init();
    myLamp9->setscaleVector(glm::vec3(0.1f));
    myLamp9->setCenter(orbit9);

    myLamp10 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp10->Init();
    myLamp10->setscaleVector(glm::vec3(0.1f));
    myLamp10->setCenter(orbit10);

    myLamp11 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp11->Init();
    myLamp11->setscaleVector(glm::vec3(0.1f));
    myLamp11->setCenter(orbit11);

    myLamp12 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp12->Init();
    myLamp12->setscaleVector(glm::vec3(0.1f));
    myLamp12->setCenter(orbit12);

    myLamp13 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp13->Init();
    myLamp13->setscaleVector(glm::vec3(0.1f));
    myLamp13->setCenter(orbit13);

    myLamp14 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp14->Init();
    myLamp14->setscaleVector(glm::vec3(0.1f));
    myLamp14->setCenter(orbit14);

    myLamp15 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp15->Init();
    myLamp15->setscaleVector(glm::vec3(0.1f));
    myLamp15->setCenter(orbit15);

    myLamp16 = new DrawScene(windowWidth, windowHeight, DrawScene::mesh::lamp);
    myLamp16->Init();
    myLamp16->setscaleVector(glm::vec3(0.1f));
    myLamp16->setCenter(orbit16);
}

void setShader()
{
    ///texture 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureShader);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureShader2);

    //camera

    vertexCamera = glGetUniformLocation(programID, "camera");
    vertexProjection = glGetUniformLocation(programID, "projection");
    vertexTransform = glGetUniformLocation(programID, "transform");
    cameraPos = glGetUniformLocation(programID, "cameraPos");
    viewPos = glGetUniformLocation(programID, "viewPos");

    glm::mat4 camera = glm::lookAt(glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(50.f), 1024.f / 800.f, 0.1f, 100.f);
    glm::mat4 transform = (glm::mat4(1.0f));

    glUniformMatrix4fv(vertexCamera, 1, GL_FALSE, &camera[0][0]);
    glUniformMatrix4fv(vertexProjection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &transform[0][0]);
    glUniform3f(cameraPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glUniform3f(viewPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    //color
    vertexColor = glGetUniformLocation(programID2, "myColor");
    glUniform3f(vertexColor, color.x, color.y, color.z);

    //light
    lightPos = glGetUniformLocation(programID, "lightPos");
    lgihtColor = glGetUniformLocation(programID, "lightColor");

    glm::vec3 lightColor(1.f);

    glUniform3f(lightPos, lightPoss.x, lightPoss.y, lightPoss.z);
    glUniform3f(lgihtColor, lightColor.x, lightColor.y, lightColor.z);

    curLightNum = glGetUniformLocation(programID, "curLightNum");
    glUniform1i(curLightNum, lightNum);

    curLightType = glGetUniformLocation(programID, "curLightType");
    glUniform1i(curLightType, lightTypes);

    mShininess = glGetUniformLocation(programID, "material.shininess");
    glUniform1f(mShininess, 32.0f);

    mDiffuse = glGetUniformLocation(programID, "material.diffuse");
    glUniform1i(mDiffuse, 0);

    mSpecular = glGetUniformLocation(programID, "material.specular");
    glUniform1i(mSpecular, 1);

    ///////////
    checkUVs = glGetUniformLocation(programID, "checkUV");
    glUniform1i(checkUVs, checkUV);

    GPUcalculations = glGetUniformLocation(programID, "GPUcalculation");
    glUniform1i(GPUcalculations, GPUcalculation);

    checkEntitys = glGetUniformLocation(programID, "checkEntity");
    glUniform1i(checkEntitys, checkEntity);

    emmisives = glGetUniformLocation(programID, "emmisive");
    glUniform3f(emmisives, emmisive.x, emmisive.y, emmisive.z);

    coefficients = glGetUniformLocation(programID, "coefficient");
    glUniform3f(coefficients, coefficient.x, coefficient.y, coefficient.z);

    gAmbients = glGetUniformLocation(programID, "gAmbient");
    glUniform3f(gAmbients, gAmbient.x, gAmbient.y, gAmbient.z);

    uvMins = glGetUniformLocation(programID, "uvMin");
    glUniform3f(uvMins, uvMin.x, uvMin.y, uvMin.z);

    uvMaxs = glGetUniformLocation(programID, "uvMax");
    glUniform3f(uvMaxs, uvMax.x, uvMax.y, uvMax.z);

    fogs = glGetUniformLocation(programID, "fog");
    glUniform3f(fogs, fog.x, fog.y, fog.z);

    fogMins = glGetUniformLocation(programID, "fogMin");
    glUniform1f(fogMins, fogMin);

    fogMaxs = glGetUniformLocation(programID, "fogMax");
    glUniform1f(fogMaxs, fogMax);

    isTextures  = glGetUniformLocation(programID, "isTexture");
    glUniform1f(isTextures, isTexture);

    uvTypeChecks = glGetUniformLocation(programID, "uvType");
    glUniform1i(uvTypeChecks, uvTypeCheck);
}

void lampSetUP()
{
    cameraPosition = glm::vec3(0.f, 1.5f, 5.0f);

    vertexCamera = glGetUniformLocation(programID2, "camera");
    vertexProjection = glGetUniformLocation(programID2, "projection");
    vertexTransform = glGetUniformLocation(programID2, "transform");
    cameraPos = glGetUniformLocation(programID2, "cameraPos");

    glm::mat4 camera = glm::lookAt(glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(50.f), 1024.f / 800.f, 0.1f, 100.f);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), lightPoss);

    glUniformMatrix4fv(vertexCamera, 1, GL_FALSE, &camera[0][0]);
    glUniformMatrix4fv(vertexProjection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(vertexTransform, 1, GL_FALSE, &transform[0][0]);
    glUniform3f(cameraPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

void setLightShader(int index)
{
    glUseProgram(programID);
    //////////////////////////

    glm::vec3 dir = -lightPosition[index];

    uniformLight = glGetUniformBlockIndex(programID, "Lights");
    glUniformBlockBinding(programID, uniformLight, 0);

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 16 * 112, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 16 *112);

    for (int i = 0; i < lightNum; i++)
    {
        directionUniform[i] = -lightPosition[i];

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112), sizeof(GLfloat), &constantUniform);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 4, sizeof(GLfloat), &linearUniform);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 8, sizeof(GLfloat), &quadraticUniform);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 12, sizeof(GLfloat), &falloffUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 16, sizeof(GLfloat), &outerAngleUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 20, sizeof(GLfloat), &innerAngleUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 24, sizeof(GLfloat), &lightType[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 32, sizeof(glm::vec3), &lightPosition[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 48, sizeof(glm::vec3), &directionUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 64, sizeof(glm::vec3), &ambientUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 80, sizeof(glm::vec3), &diffuseUniform[i]);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<long long>(i * 112) + 96, sizeof(glm::vec3), &specularUniform[i]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}