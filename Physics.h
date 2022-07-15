#pragma once
#include "Vector3.h"

class Physics
{
public:
	Vector3 Gravity;
	Physics(): Gravity(Vector3(0, -9.81f, 0)) {};
};