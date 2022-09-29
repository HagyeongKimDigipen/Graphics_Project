/*
*	hakyung.kim
*	Assigment 4
*	CS250
*	spring 2020
*/

#include "CameraRender2D.h"
#include "Projection.h"

CameraRender2D::CameraRender2D(Render& r) : render(r)
{
	world2camera = WorldToCamera(Camera());
	camera2ndc = CameraToNDC(Camera());
}

CameraRender2D::~CameraRender2D(void)
{
	cam_vertices.clear();
	ndc_vertices.clear();
}

void CameraRender2D::SetCamera(const Camera& cam)
{
	camera2ndc = CameraToNDC(cam);
	world2camera = WorldToCamera(cam);
}

void CameraRender2D::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
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
		
		if (edge1.z < 0  && edge2.z < 0)
		{
			render.DrawLine(ndc_vertices[m.GetEdge(i).index1], ndc_vertices[m.GetEdge(i).index2]);
		}
	}
}

void CameraRender2D::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
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

	for (int i = 0; i < m.VertexCount(); i++)
	{
		vertices = A * m.GetVertex(i);

		world_verices = world2camera * vertices;
		vertices = Point(world_verices.x / world_verices.w, world_verices.y / world_verices.w, world_verices.z / world_verices.w );
		cam_vertices.push_back(vertices);

		ndc_camera = camera2ndc * cam_vertices[i];
		ndc_vertices.push_back(Point(ndc_camera.x / ndc_camera.w, ndc_camera.y / ndc_camera.w, ndc_camera.z / ndc_camera.w));
	}

	for (int i = 0; i < m.FaceCount(); i++)
	{
		orientation = cross(cam_vertices[m.GetFace(i).index2] - cam_vertices[m.GetFace(i).index1], cam_vertices[m.GetFace(i).index3] - cam_vertices[m.GetFace(i).index1]);
		color_u = abs(dot(L, orientation)) / (abs(L) * abs(orientation));
		is_visible = dot(E - cam_vertices[m.GetFace(i).index1], orientation);
		real_color = color_u * color;

		render.SetColor(real_color);

		if (is_visible >= 0)
		{
			if (cam_vertices[m.GetFace(i).index1].z < 0 && cam_vertices[m.GetFace(i).index2].z < 0 && cam_vertices[m.GetFace(i).index3].z < 0)
			{
				render.FillTriangle(ndc_vertices[m.GetFace(i).index1], ndc_vertices[m.GetFace(i).index2], ndc_vertices[m.GetFace(i).index3]);
			}
		}
	}

}

