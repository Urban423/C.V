#pragma once
#include <vector>
#include "SphereCollider.h"
#include "GameObject.h"
#include "Game.h"

struct RaycastHit
{
	float distance;
	GameObject* hitObject;
	Vector3 point;
	Vector3 normal;
};

class Physics
{
public:
	Vector3 Gravity;
	static bool Raycast(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit);
public:
	Physics();
	~Physics();
private:
	static bool boxIntersection(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit, GameObject* box);
	static bool sphereIntersection(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit, GameObject* sphere);
	static bool RayIntersectsTriangle(Vector3 rayOrigin, Vector3 rayVector, Vector3 vertex0, Vector3 vertex1, Vector3 vertex2, Vector3& outIntersectionPoint);
	static void trinaglehelper(Vector3 rayOrigin, Vector3 rayVector, float distance, int i1, int i2, int i3, RaycastHit* hit, Vector3* list, bool& ret, GameObject* obj);

	friend class Game;
};