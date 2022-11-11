#pragma once
#include "Vector3.h"

class Vector4
{
public:
	Vector4() :x(0), y(0), z(0), w(0)
	{
	}
	Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w)
	{
	}
	Vector4(const Vector4& vector) :x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}
	Vector4(const Vector3& vector) :x(vector.x), y(vector.y), z(vector.z), w(1)
	{
	}
	Vector4(const Vector3& vector, const float& w) :x(vector.x), y(vector.y), z(vector.z), w(w)
	{
	}
	void cross(Vector4& v1, Vector4& v2, Vector4& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}
public:
	float x, y, z, w;
};
