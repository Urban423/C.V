#include "MeshCollider.h"
#include "Mesh.h"

MeshCollider::MeshCollider()
{

}

Vector3 MeshCollider::OnCollissionEnter()
{
	mesh->getVertexBuffer()->getSizeVertexList();

	return Vector3();
}

MeshCollider::~MeshCollider()
{

}