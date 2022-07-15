#pragma once
#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
	VertexMesh() :position(), texcoord(), normal()
	{
	}
	VertexMesh(Vector3 position, Vector2 texcoord, Vector3 normal) :position(position), texcoord(texcoord), normal(normal)
	{
	}
	VertexMesh(const VertexMesh& vertex) :position(vertex.position), texcoord(vertex.texcoord), normal(vertex.normal)
	{
	}
	~VertexMesh()
	{
	}
public:
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;
};