#pragma once
#include "Component.h"

class Rigidbody: public Component
{
public:
	bool isKinematic = false;
	float mass = 1;
	bool idle = false;
	Vector3 lastpos;
public:
	Rigidbody() {};
	~Rigidbody() {};
};