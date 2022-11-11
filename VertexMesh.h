#pragma once
#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
	VertexMesh() :position(), texcoord(), normal()
	{
	}
	VertexMesh(const Vector3& n_position, const Vector2& n_texcoord, const Vector3& n_normal,
		const Vector3& n_tangent, const Vector3& n_binormal)
	{
		position = n_position;
		texcoord = n_texcoord;
		normal = n_normal;
		tangent = n_tangent;
		binormal = n_binormal;
	}
	VertexMesh(const VertexMesh& n_vertex)
	{
		position = n_vertex.position;
		texcoord = n_vertex.texcoord;
		normal = n_vertex.normal;
		tangent = n_vertex.tangent;
		binormal = n_vertex.binormal;
	}
	~VertexMesh()
	{
	}
public:
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;
	Vector3 tangent;
	Vector3 binormal;
};