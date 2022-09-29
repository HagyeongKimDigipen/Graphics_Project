/*
*	hakyung.kim
*	Assigment 3
*	CS250
*	spring 2020
*/

#include "FrustumMesh.h"

const Mesh::Face FrustumMesh::faces[12] = { Mesh::Face(0,1,8),Mesh::Face(1,2,8),Mesh::Face(2,3,8),Mesh::Face(3,0,8),Mesh::Face(4,5,8),Mesh::Face(5,6,8),Mesh::Face(7,4,8),Mesh::Face(4,5,6),Mesh::Face(7,4,6),Mesh::Face(0,1,2),Mesh::Face(3,0,2),Mesh::Face(7,8,6) };
const Mesh::Edge FrustumMesh::edges[16] = { Mesh::Edge(0,1),Mesh::Edge(1,2),Mesh::Edge(2,3),Mesh::Edge(4,5),Mesh::Edge(5,6),Mesh::Edge(6,7),Mesh::Edge(7,4),Mesh::Edge(1,5),Mesh::Edge(2,6),Mesh::Edge(3,7),Mesh::Edge(0,4),Mesh::Edge(5,8),Mesh::Edge(6,8),Mesh::Edge(8,4),Mesh::Edge(7,8),Mesh::Edge(0,3) };

FrustumMesh::FrustumMesh(float fov, float a, float n, float f)
{
	float Wn = 2 * n * tan(fov / 2);
	float Hn = Wn / a;
	float Wf = 2 * f * tan(fov / 2);
	float Hf = Wf / a;

	vertices[0] = Point{ -Wf / 2,Hf / 2,-f };
	vertices[1] = Point{ -Wf / 2,-Hf / 2,-f };
	vertices[2] = Point{ Wf / 2,-Hf / 2,-f };
	vertices[3] = Point{ Wf / 2,Hf / 2,-f };

	vertices[4] = Point{ -Wn / 2,Hn / 2,-n };
	vertices[5] = Point{ -Wn / 2,-Hn / 2,-n };
	vertices[6] = Point{ Wn / 2,-Hn / 2,-n };
	vertices[7] = Point{ Wn / 2,Hn / 2,-n };

	vertices[8] = Point{ 0,0,0 };
}

int FrustumMesh::VertexCount(void)
{
	return sizeof(vertices) / sizeof(vertices[0]);
}

Point FrustumMesh::GetVertex(int i)
{
	return vertices[i];
}

Vector FrustumMesh::Dimensions(void)
{
	return dimensions;
}

Point FrustumMesh::Center(void)
{
	return center;
}

int FrustumMesh::FaceCount(void)
{
	return sizeof(faces) / sizeof(faces[0]);
}

Mesh::Face FrustumMesh::GetFace(int i)
{
	return faces[i];
}

int FrustumMesh::EdgeCount(void)
{
	return sizeof(edges) / sizeof(edges[0]);
}

Mesh::Edge FrustumMesh::GetEdge(int i)
{
	return edges[i];
}
