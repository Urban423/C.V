#pragma once
#include "UrbanEngine.h"
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Rotate: public Component
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
	Vector3 rotate;
public:
	Rotate(): Component()
	{
		rotate = Vector3();
	}

	Rotate(Vector3 r) : Component()
	{
		rotate = r;
	}

	void Update()
	{
		x += rotate.x * Time.delta_time;
		y += rotate.y * Time.delta_time;
		z += rotate.z * Time.delta_time;

		if (Input.keyDown == 81)
		{
			x = 0;
			y = 0;
			z = 0;
		}

		gameobject.transform.rotation = Vector3(x, y, z);
	};

	~Rotate()
	{

	}
};