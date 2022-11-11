#pragma once
#include "GameObject.h"
#include "Color.h"

class Light: public Component
{
public:
	Light()
	{
		color = Color(1, 1, 1, 1);
		radius = 10;
	}
	Light(Color color, float distance)
	{
		this->color = color;
		this->radius = 1;
	}
	~Light() {};
public:
	Color color;
	float radius;
};