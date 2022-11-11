#pragma once
#include "Vector3.h"
#include "Vector4.h"

class Color
{
public:
	Color()
	{
		r = 1;
		g = 1;
		b = 1;
		a = 1;
	}
	Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		a = 1;
	}

	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	~Color() {};

	Vector3 Color3()
	{
		return Vector3(r, g, b);
	}
	Vector4 Color4()
	{
		return Vector4(r, g, b, a);
	}
public:
	float r;
	float g;
	float b;
	float a;
};