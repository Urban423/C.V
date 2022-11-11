#pragma once
#include "BoxCollider.h"

class SphereCollider: public Component
{
public:
	float radius;
	Vector3 Center;
	bool isTrigger;
public:
	SphereCollider()
	{
		isTrigger = false;
		radius = 1;
		Center = Vector3(0, 0, 0);
	}
	SphereCollider(float radius, Vector3 Center)
	{
		this->radius = radius;
		isTrigger = false;
		this->Center = Center;
	}
	static Vector3 onCollisonEnter(GameObject first, GameObject second);
private:
	friend class BoxCollider;
};

