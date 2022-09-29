/*
*	hakyung.kim
*	Assigment 2
*	CS250
*	spring 2020
*/

#include "CubeMesh.h"

const Point CubeMesh::vertices[8] = { Point::Point(1,1,1),Point::Point(-1,1,1),Point::Point(1,1,-1),Point::Point(-1,1,-1),Point::Point(1,-1,1),Point::Point(-1,-1,1),Point::Point(1,-1,-1),Point::Point(-1,-1,-1) };
const Mesh::Face CubeMesh::faces[12] = { Mesh::Face(0,4,6),Mesh::Face(0,6,2),Mesh::Face(1,3,7),Mesh::Face(1,7,5),Mesh::Face(4,5,7),Mesh::Face(4,7,6),Mesh::Face(0,2,3),Mesh::Face(0,3,1),Mesh::Face(0,1,4),Mesh::Face(1,5,4),Mesh::Face(2,6,3),Mesh::Face(3,6,7) };
const Mesh::Edge CubeMesh::edges[12] = { Mesh::Edge(0,1),Mesh::Edge(0,2),Mesh::Edge(0,4),Mesh::Edge(1,5),Mesh::Edge(1,3),Mesh::Edge(5,4),Mesh::Edge(5,7),Mesh::Edge(4,6),Mesh::Edge(2,3),Mesh::Edge(2,6),Mesh::Edge(3,7),Mesh::Edge(7,6) };

int CubeMesh::VertexCount(void)
{
	return sizeof(vertices) / sizeof(vertices[0]);
}

Point CubeMesh::GetVertex(int i)
{
	return vertices[i];
}

Vector CubeMesh::Dimensions(void)
{
	return Vector( 2,2,2 );
}

Point CubeMesh::Center(void)
{
	return Point( 0,0,0 );
}

int CubeMesh::FaceCount(void)
{
	return sizeof(faces) / sizeof(faces[0]);
}

Mesh::Face CubeMesh::GetFace(int i)
{
	return faces[i];
}

int CubeMesh::EdgeCount(void)
{
	return sizeof(edges) / sizeof(edges[0]);
}

Mesh::Edge CubeMesh::GetEdge(int i)
{
	return edges[i];
}
