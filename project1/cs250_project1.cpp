// cs250_project1.empty.cpp
// -- boiler plate with frame rate counter
// cs250 10/17

/*
*	hakyung.kim
*	Project Assignment 01
*	CS250
*	spring 2020
*/

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include "Affine.h"

#include "Affine.h"
#include "Mesh.h"
#include "CubeMesh.h"
#include "SnubDodecMesh.h"
#include "CameraRender2D.h"
#include "Camera.h"
#include "Projection.h"
#include "Render.h"
#include "FrustumMesh.h"
#include <vector>
#include "math.h"  

using namespace std;

const Point O(0, 0, 0);

const Vector EX(1, 0, 0),
EY(0, 1, 0),
EZ(0, 0, 1);

const float PI = 4.0f * atan(1.0f);

const Vector WHITE(1, 1, 1),
PURPLE(1, 0, 1),
BLACK(0, 0, 0),
RED(1, 0, 0),
GREEN(0, 1, 0),
BLUE(0, 0, 1),
PINK(1, 0.752f, 0.796f),
HOOD(0.6784f, 0.847f, 0.901f),
FACE_ORANGE(1, 0.854f, 0.725f),
ORANGE(1, 0.647f, 0),
GRAY(0.8f, 0.8f, 0.8f),
BACKGROUND(1,0.94117f,0.96078f),
LAVENDER(0.9019f,0.9019f,0.9803f),
LIGHTYELLOW(1,0.9803f,0.8039f);

class Heart : public Mesh {
public:
    int VertexCount(void) { return 18; }
    Point GetVertex(int i) { return vertices[i]; }
    Vector Dimensions(void) { return Vector(2, 2, 2); }
    Point Center(void) { return Point(0, 0, 0); }
    int FaceCount(void) { return 30; }
    Face GetFace(int i) { return faces[i]; }
    int EdgeCount(void) { return 49; }
    Edge GetEdge(int i) { return edges[i]; }
private:
    static Point vertices[18];
    static Face faces[30];
    static Edge edges[49];
};

Point Heart::vertices[18] = { Point(-1,0.75f,0.5f),Point(0,0.75f,0.5f),Point(0,0.75f,-0.5f),Point(-1,0.75f,-0.5f),Point(-0.5f,1,-0.5f),Point(-0.5f,1,0.5f),Point(1,0.75f,0.5f),Point(1,0.75f,-0.5f),Point(0.5f,1,0.5f),Point(0.5f,1,-0.5f), Point(0,0,0.5),Point(0.f,0.f, -0.5f),Point(-1,-0.25f,0.5f),Point(-1,-0.25f,-0.5f),Point(1,-0.25f,-0.5f),Point(1,-0.25f,0.5f),Point(0,-1,0.5f),Point(0,-1,-0.5f) };
Mesh::Face Heart::faces[30] = { Face(0,1,5),Face(3,4,2),Face(0,5,4),Face(0,4,3),Face(4,5,1),Face(1,2,4),Face(1,6,8),Face(2,9,7),Face(1,8,9),Face(9,2,1),Face(9,8,7),Face(8,6,7),Face(10,6,0),Face(0,12,10),Face(12,15,10),Face(15,6,10),Face(3,7,11),Face(3,11,13),Face(13,11,14),Face(14,11,7),Face(12,0,3),Face(12,3,13),Face(7,6,15),Face(15,14,7),Face(16,12,13),Face(13,17,16),Face(14,15,16),Face(16,17,14),Face(16,15,12),Face(17,13,14) };
Mesh::Edge Heart::edges[49] = {Edge(0,1),Edge(0,2),Edge(0,3),Edge(0,4),Edge(0,5),Edge(5,4),Edge(1,5),Edge(2,4) ,Edge(1,2),Edge(1,4),Edge(2,3),Edge(3,4),Edge(1,6),Edge(7,1),Edge(8,9),Edge(1,8),Edge(2,9),Edge(6,8),Edge(6,9),Edge(9,7),Edge(2,7),Edge(6,7),Edge(1,9),Edge(0,12),Edge(3,12),Edge(3,13),Edge(7,14),Edge(7,15),Edge(6,15),Edge(0,10),Edge(12,10),Edge(6,10),Edge(15,10),Edge(3,11),Edge(7,11),Edge(13,11),Edge(14,11),Edge(13,12),Edge(13,16),Edge(13,7),Edge(13,14),Edge(16,12),Edge(16,17),Edge(16,14),Edge(14,15),Edge(14,17),Edge(15,12),Edge(14,12),Edge(16,15) };

struct ComputeSpline
{
    vector<Vector> points;

    Vector SplinePoint(float value, bool check = false)
    {
        int p0, p1, p2, p3;
        if (!check)
        {
            p1 = (int)value + 1;
            p2 = p1 + 1;
            p3 = p2 + 1;
            p0 = p1 - 1;
        }
        else
        {
            p1 = (int)value;
            p2 = (p1 + 1) % static_cast<int>(points.size());
            p3 = (p2 + 1) % static_cast<int>(points.size());
            p0 = p1 >= 1 ? p1 - 1 : static_cast<int>(points.size()) - 1;
        }

        value = value - (int)value;

        float square_value = value * value;
        float triple_value = square_value * value;

        float q1 = -triple_value + 2.0f * square_value - value;
        float q2 = 3.0f * triple_value - 5.0f * square_value + 2.0f;
        float q3 = -3.0f * triple_value + 4.0f * square_value + value;
        float q4 = triple_value - square_value;

        float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
        float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);
        float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

        return Vector(tx, ty, tz);
    }
};

class RenderMesh
{
public:
    RenderMesh(Render& r) : render(r) {
        world2camera = WorldToCamera(Camera());
        camera2ndc = CameraToNDC(Camera());
    }

    ~RenderMesh(void) {
        cam_vertices.clear();
        ndc_vertices.clear();
    }
    void SetCamera(const Camera& cam) {
        camera2ndc = CameraToNDC(cam);
        world2camera = WorldToCamera(cam);
    }
    void DisplayEdges(Mesh& m, const Affine& A, const Vector& color) {
        cam_vertices.clear();
        ndc_vertices.clear();

        Hcoord edge1;
        Hcoord edge2;

        Hcoord vertices;

        Matrix to_ndc = camera2ndc * world2camera;

        float w_value;

        render.SetColor(color);

        for (int i = 0; i < m.VertexCount(); i++)
        {
            vertices = A * m.GetVertex(i);
            cam_vertices.push_back(vertices);
            vertices = to_ndc * cam_vertices[i];

            w_value = 1 / vertices.w;
            ndc_vertices.push_back(Point(vertices.x * w_value, vertices.y * w_value, vertices.z * w_value));
        }

        for (int i = 0; i < m.EdgeCount(); i++)
        {
            edge1 = world2camera * cam_vertices[m.GetEdge(i).index1];
            edge2 = world2camera * cam_vertices[m.GetEdge(i).index2];

            if (edge1.z < 0 && edge2.z < 0)
            {
                render.DrawLine(ndc_vertices[m.GetEdge(i).index1], ndc_vertices[m.GetEdge(i).index2]);
            }
        }
    }
    void DisplayFaces(Mesh& m, const Affine& A, const Vector& color) {
        cam_vertices.clear();
        ndc_vertices.clear();

        Point E(0, 0, 0);
        Hcoord L(0, 0, 1);

        Hcoord ndc_camera;
        Point vertices;


        Vector orientation;
        Vector real_color;

        float is_visible = 0;
        float color_u = 0.f;

        Hcoord world_verices;

        Matrix toCamera = world2camera * A;

        for (int i = 0; i < m.VertexCount(); i++)
        {
            vertices = toCamera * m.GetVertex(i);
            cam_vertices.push_back(vertices);
            ndc_camera = camera2ndc * cam_vertices[i];
            ndc_vertices.push_back(Hcoord(ndc_camera.x , ndc_camera.y , ndc_camera.z, ndc_camera.w));
        }

        for (int i = 0; i < m.FaceCount(); i++)
        {
            orientation = cross(cam_vertices[m.GetFace(i).index2] - cam_vertices[m.GetFace(i).index1], cam_vertices[m.GetFace(i).index3] - cam_vertices[m.GetFace(i).index1]);
            is_visible = dot(E - cam_vertices[m.GetFace(i).index1], orientation);
            color_u = abs(dot(L, orientation)) / (abs(L) * abs(orientation));
            real_color = color_u * color;
            render.SetColor(real_color);

           if (cam_vertices[m.GetFace(i).index1].z < 0 || cam_vertices[m.GetFace(i).index2].z < 0 || cam_vertices[m.GetFace(i).index3].z < 0)
           {
               if (is_visible >= 0)
               {
                   render.FillTriangle(ndc_vertices[m.GetFace(i).index1], ndc_vertices[m.GetFace(i).index2], ndc_vertices[m.GetFace(i).index3]);
               }
           }        
        }
    }

private:
    Render& render;
    Affine world2camera;
    Matrix camera2ndc;
    std::vector<Point> cam_vertices;
    std::vector<Hcoord> ndc_vertices;
};

class DrawSpline
{
public:
    DrawSpline() {};

    void makePoint()
    {
        path.points = {
            Vector(0.5f, -0.6f, -2),
            Vector(-1.2f, 1.32f, -0.5f),
            Vector(1.3f, -0.5f, -0.25f),
            Vector(0.7f, -0.8f, -3),
            Vector(-1.5f, 0.2f, -0.8f),
            Vector(-1, 1, -0.22f),
            Vector(-0.5f, -0.35f, -0.2f),
            Vector(0.3f, -0.2f, -0.1f),
            Vector(-0.5f, 0.15f, -1.2f),
            Vector(-0.2f, 1.5f, -2),
            Vector(0.23f, -0.35f, -3.2f),
            Vector(-0.12f, 0.35f, -0.5f),
            Vector(0.5f, 0.15f, -1.2f),
            Vector(0.2f, 0.25f, -2),
            Vector(0.23f, -0.35f, -3.2f),
            Vector(-0.12f, 0.35f, -0.5f)
        };
    }


    vector<Vector> makeVector()
    {
        vector<Vector> path_vector;
        if (point_marker >= (float)path.points.size())
        {
            point_marker -= (float)path.points.size();
        }

        if (point_marker < 0.0f)
        {
            point_marker += (float)path.points.size();

        }

        for (float t = 0; t < (float)path.points.size(); t += 0.003f)
        {
            path_vector.push_back(path.SplinePoint(t, true));
            count++;
        }

        return path_vector;
    }

    int GetCurrCount() 
    {
        if (count - 2 < counter)
        {
            counter = 0;
        }
        else
        {
            counter++;
        }
        return counter;
    }

    ComputeSpline GetPath()
    {
        return path;
    }

private:
    ComputeSpline path;
    float point_marker = 0.0f;
    int count = 0;
    int counter = 0;
};

class Client
{
public:
    Client(SDL_Window* window);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);

private:
    // variables for frame rate:
    SDL_Window* window;
    int frame_count;
    double frame_time;

    // other variables:
    Camera cam1, cam2, cam3;
    SnubDodecMesh circle_1_mesh;
    CubeMesh cube_mesh;
    Affine cube2world[25],world_affine, heart2world[16], heart_world, camera_affine;
    float snub_rot_rate;
    Vector snub_rot_axis;
    Point snub_center;
    float mesh_scale, mesh_rot_rate;
    Vector mesh_rot_axis;
    bool draw_solid;
    Render* render;
    RenderMesh* to_render;
    Heart heart_render;
    double time;
    int cameraCount = 1;
    bool use_cam1, use_cam2, use_cam3;
    bool printF = true;
    DrawSpline to_spline;
    vector<Vector> spline_vector;
};

Client::Client(SDL_Window* win) : window(win)
{
    frame_count = 0;
    frame_time = 0;
    snub_center = Point(0, 0, -2);

    // initialization code:
    render = new Render();
    cam1 = Camera(O + EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 10);
    cam2 = Camera(O + EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 10);
    cam3 = Camera(O + EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 10);

    to_render = new RenderMesh(*render);

    snub_rot_rate = 2 * PI / 10.0f;
    snub_rot_axis = -EY;

    mesh_scale = 2.0f;
    mesh_rot_rate = 2 * PI / 10.0f;
    mesh_rot_axis = normalize(Vector(1, 1, 1));
    draw_solid = false;

    use_cam1 = true;
    use_cam2 = false;
    use_cam3 = false;

    time = 0;

    world_affine = translate(snub_center - O);
    heart_world = translate(snub_center - O);

    for (int i = 0; i < 25; ++i)
    {
        cube2world[i] = translate(Vector(float((i % 5) - 1.8f), -1.1f, float(-(i / 5) - 0.8f)))
            * scale(0.4f, 0.05f, 0.2f);
    }

    for (int i = 0; i < 16; ++i)
    {
        heart2world[i] = translate(Vector(float((i % 4) - 1.5f), 1.1f, float(-(i / 4) - 0.5f)))
            * scale(0.3f, 0.3f, 0.2f);
    }

    to_spline.makePoint();
    spline_vector = to_spline.makeVector();

    camera_affine = translate(Vector(spline_vector[0]));

    glDisable(GL_DEPTH_TEST);
}


Client::~Client(void)
{
    // clean-up code:
    delete render;
    delete to_render;
}


void Client::draw(double dt)
{
    if (printF)
    {
        //std::cout << "if you press the F key, you can see shape that filled by color" << std::endl;
        printF = false;
    }

    render->ClearBuffers(BACKGROUND);

    if (use_cam1)
    {
        to_render->SetCamera(cam1);
    }
    else if (use_cam2)
    {   
        cam2 = Camera(Point(Hcoord(camera_affine[0].w, camera_affine[1].w, camera_affine[2].w, 1)), EZ, EY, 0.5f * PI, 1, 0.01f, 10);
        to_render->SetCamera(cam2);
    }
    else if (use_cam3)
    {
        cam3 = Camera(Point(Hcoord(camera_affine[0].w, camera_affine[1].w, camera_affine[2].w, 1)), snub_center - Point(Hcoord(camera_affine[0].w, camera_affine[1].w, camera_affine[2].w, 1)), EY, 0.5f * PI, 1, 0.01f, 10);
        to_render->SetCamera(cam3);
    }

    //   dt: time (in seconds) since last animation frame
    // frame rate:
    ++frame_count;
    frame_time += dt;
    if (frame_time >= 0.5)
    {
        double fps = frame_count / frame_time;
        frame_count = 0;
        frame_time = 0;
        stringstream ss;
        ss << "CS 250: Project #1 [fps=" << int(fps) << "]";
        SDL_SetWindowTitle(window, ss.str().c_str());
    }

    // drawing code:

    //cube
    for (int i = 0; i < 25; ++i)
    {
        if (draw_solid)
        {
            if (i % 2 == 0)
            {
                to_render->DisplayFaces(cube_mesh, cube2world[i], LAVENDER);
            }
            else
            {
                to_render->DisplayFaces(cube_mesh, cube2world[i], LIGHTYELLOW);
            }
        } 
        else
        {
            if (i % 2 == 0)
            {
                to_render->DisplayEdges(cube_mesh, cube2world[i], LAVENDER);
            }
            else
            {
                to_render->DisplayEdges(cube_mesh, cube2world[i], LIGHTYELLOW);

            }
        }
    }

    //heart
    for (int i = 0; i < 16; ++i)
    {
        if (draw_solid)
        {
            if (i % 2 == 0)
            {
                to_render->DisplayFaces(heart_render, heart2world[i], LAVENDER);
            }
            else
            {
                to_render->DisplayFaces(heart_render, heart2world[i], LIGHTYELLOW);
            }
        }
        else
        {
            if (i % 2 == 0)
            {
                to_render->DisplayEdges(heart_render, heart2world[i], LAVENDER);
            }
            else
            {
                to_render->DisplayEdges(heart_render, heart2world[i], LIGHTYELLOW);

            }
        }
    }

    //circle

    //ear right
    world_affine = translate(snub_center - Point(-1, -1, 0)) * scale(2.0f / 5);

    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, HOOD);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, HOOD);
    }

    //ear left
    world_affine = translate(snub_center - Point(1, -1, 0)) * scale(2.0f / 5);

    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, HOOD);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, HOOD);
    }

    //////////////////////////////////////////////////////////////////////
    //heart
    heart_world = translate(snub_center - Point(cos(static_cast<float>(time)) * 1.8f, 0, sin(static_cast<float>(time)) * 1.8f))* scale(0.3f);

    if (sin(time) * 1.8f > 0)
    {
        if (draw_solid)
        {
            to_render->DisplayFaces(heart_render, heart_world, PINK);
        }
        else
        {
            to_render->DisplayEdges(heart_render, heart_world, PINK);
        }
    }

    //big hair
    world_affine = translate(snub_center - O) * scale(1.3f);

    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, HOOD);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, HOOD);
    }


    world_affine = translate(snub_center - O) * rotate(snub_rot_rate * static_cast<float>(time), snub_rot_axis);

    //face
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, ORANGE);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, ORANGE);
    }

    /////////////////////////////////////////////////////////////////////
    //SUYUM1
    world_affine = translate(snub_center -Point(-0.15f, 0.35f, 0)) * scale(0.2f);
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, WHITE);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, WHITE);
    }

    //SUYUM2
    world_affine = translate(snub_center - Point(0.15f, 0.35f, 0)) * scale(0.2f);
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, WHITE);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, WHITE);
    }

    //nose
    world_affine = translate(snub_center - Point(0, 0.2f, 0)) *  scale(0.15f);
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, BLACK);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, BLACK);
    }

    //eye1
    world_affine = translate(snub_center - Point(-0.4f, -0.18f, 0)) * scale(0.13f);
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, BLACK);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, BLACK);
    }

    //eye2
    world_affine = translate(snub_center - Point(0.4f, -0.18f, 0)) * scale(0.13f);
    if (draw_solid)
    {
        to_render->DisplayFaces(circle_1_mesh, world_affine, BLACK);
    }
    else
    {
        to_render->DisplayEdges(circle_1_mesh, world_affine, BLACK);
    }

    //eyebrow1
    world_affine = translate(snub_center - Point(-0.4f, -0.4f, 0)) * scale(0.2f, 0.04f, 0.01f);
    if (draw_solid)
    {
        to_render->DisplayFaces(cube_mesh, world_affine, BLACK);
    }
    else
    {
        to_render->DisplayEdges(cube_mesh, world_affine, BLACK);
    }

    //eyebrow2
    world_affine = translate(snub_center - Point(0.4f, -0.4f, 0)) * scale(0.2f, 0.04f, 0.01f);
    if (draw_solid)
    {
        to_render->DisplayFaces(cube_mesh, world_affine, BLACK);
    }
    else
    {
        to_render->DisplayEdges(cube_mesh, world_affine, BLACK);
    }

    //heart
    if (sin(time) * 1.8f <= 0)
    {
        if (draw_solid)
        {
            to_render->DisplayFaces(heart_render, heart_world, PINK);
        }
        else
        {
            to_render->DisplayEdges(heart_render, heart_world, PINK);
        }
    }

    ///////////////////////////////

    int index = to_spline.GetCurrCount();

    float dot_value;
    float normal_1_mag;
    float normal_2_mag;
    float angle = 0;

    Vector vector1;
    Vector vector2;

    Vector cross_value;

    if (index != 0)
    {
        dot_value = dot(spline_vector[index - 1], spline_vector[index]);
        normal_1_mag = sqrt(spline_vector[index - 1].x * spline_vector[index - 1].x + spline_vector[index - 1].y * spline_vector[index - 1].y + spline_vector[index - 1].z * spline_vector[index - 1].z);
        normal_2_mag = sqrt(spline_vector[index].x * spline_vector[index].x + spline_vector[index].y * spline_vector[index].y + spline_vector[index].z * spline_vector[index].z);
        
        angle = acos(dot_value / (normal_1_mag * normal_2_mag));
        
        //vector1 = to_spline.GetPath().points[index] - to_spline.GetPath().points[index - 1];
        //vector2 = to_spline.GetPath().points[index + 1] - to_spline.GetPath().points[index - 1];

        //cross_value = cross(vector1, vector2);
    }

    //std::cout << angle << std::endl;
    //std::cout << cross_value.x << cross_value.y << cross_value.z << std::endl;

   camera_affine = translate(spline_vector[index]) * rotate(angle, mesh_rot_axis)* scale(0.2f, 0.2f, 0.2f);

   if (draw_solid)
   {
       to_render->DisplayFaces(heart_render, camera_affine, RED);

   }
   else
   {
       to_render->DisplayEdges(heart_render, camera_affine, RED);
   }


    time += dt;
}


void Client::keypress(SDL_Keycode kc)
{

    // respond to keyboard input
    // kc: SDL keycode (e.g., SDLK_SPACE, SDLK_a, SDLK_s)

    const float angle_increment = PI / 180.0f,
        dist_increment = 0.1f,
        zoom_increment = 0.95f;

    if (kc == SDLK_SPACE)
    {
        cameraCount++;

        if (cameraCount > 3)
        {
            cameraCount = 1;
        }
    }

    if (cameraCount == 1)
    {
        use_cam1 = true;
        use_cam2 = false;
        use_cam3 = false;
    }
    else if (cameraCount % 2 == 0)
    {
        use_cam1 = false;
        use_cam2 = true;
        use_cam3 = false;
    }
    else if (cameraCount % 3 == 0)
    {
        use_cam1 = false;
        use_cam2 = false;
        use_cam3 = true;
    }

    if (kc == SDLK_f)
    {
        draw_solid = !draw_solid;
    }
}


void Client::resize(int W, int H)
{
    // respond to window resize
    //   W,H: window width and height (in pixels)
    glViewport(0, 0, W, H);
    float aspect = float(W) / float(H);
    cam1 = Camera(cam1.Eye(), -cam1.Back(), EY, 0.5f * PI, aspect, 0.01f, 10);
    cam2 = Camera(cam2.Eye(), -cam2.Back(), EY, 0.5f * PI, aspect, 0.01f, 10);
    cam3 = Camera(cam3.Eye(), -cam3.Back(), EY, 0.5f * PI, aspect, 0.01f, 10);

}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int /*argc*/, char* /*argv*/[])
{
    srand(unsigned(time(0)));

    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    const char* title = "CS250: Project #1";
    int width = 600, height = 600;
    SDL_Window* window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // GLEW: get function bindings (if possible)
    glewInit();
    if (!GLEW_VERSION_2_0)
    {
        cout << "needs OpenGL version 3.0 or better" << endl;
        return -1;
    }

    // animation loop
    try
    {
        bool done = false;
        Client* client = new Client(window);
        Uint32 ticks_last = SDL_GetTicks();
        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        done = true;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            done = true;
                        else
                            client->keypress(event.key.keysym.sym);
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                            client->resize(event.window.data1, event.window.data2);
                        break;
                }
            }
            Uint32 ticks = SDL_GetTicks();
            double dt = 0.001 * (ticks - ticks_last);
            ticks_last = ticks;
            client->draw(dt);
            SDL_GL_SwapWindow(window);
        }
        delete client;
    }

    catch (exception& e)
    {
        cout << e.what() << endl;
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}
