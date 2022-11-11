#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Collider.h"
#include "GameObject.h"

class BoxCollider: public Component
{
public:
	Vector3 size;
	Vector3 Center;
	bool isTrigger;
public:
	BoxCollider()
	{
		isTrigger = false;
		size = Vector3(1, 1, 1);
		Center = Vector3(0, 0, 0);
	}
	BoxCollider(Vector3 s, Vector3 c)
	{
		isTrigger = false;
		size = s;
		Center = c;
	}
	static Vector3 onCollisonEnter(GameObject first, GameObject second);
	static Vector3* GetPoint(BoxCollider* box, Quaternion qu, Vector3 worldpos);
private:
	static float ProjVector3(Vector3 v, Vector3 a);
	static Vector2 ProjAxis(Vector3 points[], Vector3 Axis);
	static Vector3 IntersectionOfProj(Vector3 a[], Vector3 b[], Vector3 Axis[]);
	static void GetAxis(Vector3 a[], Vector3 b[], Vector3* result);
};

