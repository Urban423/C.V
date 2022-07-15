#pragma once
#include "Component.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Prerequisites.h"

class MeshCollider
{
public:
	bool isTrigger;
	MeshPtr mesh;
private:
	MeshCollider();

	Vector3 OnCollissionEnter();

	~MeshCollider();
};