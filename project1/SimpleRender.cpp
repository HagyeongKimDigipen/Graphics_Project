/*
*	hakyung.kim
*	Assigment 3
*	CS250
*	spring 2020
*/

#include "SimpleRender.h"

SimpleRender::SimpleRender(Render& r) : render(r)
{
	PersProj.row[0].x = 11;
	PersProj.row[1].y = 11;
	PersProj.row[2].z = 1;
	PersProj.row[2].w = -5;
	PersProj.row[3].z = -2;
	PersProj.row[3].w = 10;
}

SimpleRender::~SimpleRender(void)
{
	render.~Render();
}

void SimpleRender::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
	Hcoord edge1;
	Hcoord edge2;

	for (int count = 0; count < m.EdgeCount(); count++)
	{
		edge1 = (1 / (PersProj * (A * m.GetVertex(m.GetEdge(count).index1))).w) * (PersProj * (A * m.GetVertex(m.GetEdge(count).index1)));
		edge2 = (1 / (PersProj * (A * m.GetVertex(m.GetEdge(count).index2))).w) * (PersProj * (A * m.GetVertex(m.GetEdge(count).index2)));


		render.SetColor(color);
		render.DrawLine(edge1, edge2);
	}
}

void SimpleRender::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
	Hcoord L(0, 0, 1);
	Point E(0, 0, 5);

	Point P;
	Point Q;
	Point R;

	Vector orientation;
	Vector real_color;

	float is_visible;
	float color_u;

	for (int i = 0; i < m.FaceCount(); ++i)
	{
		P = A * m.GetVertex(m.GetFace(i).index1);
		Q = A * m.GetVertex(m.GetFace(i).index2);
		R = A * m.GetVertex(m.GetFace(i).index3);

		orientation = cross((Q - P), (R - P));
		is_visible = dot(E - P, orientation);

		color_u = abs(dot(L, orientation)) / (abs(L) * abs(orientation));
		real_color = color_u * color;

		render.SetColor(real_color);

		P = (1 / (PersProj * P).w) * (PersProj * P);
		Q = (1 / (PersProj * Q).w) * (PersProj * Q);
		R = (1 / (PersProj * R).w) * (PersProj * R);

		if (is_visible > 0)
		{
			render.FillTriangle(P, Q, R);
		}
	}
}