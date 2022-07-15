#pragma once
#include <memory>

class Vector3
{
public:
	Vector3():x(0), y(0), z(0)
	{
	}
	Vector3(float x, float y, float z) :x(x), y(y), z(z)
	{
	}
	Vector3(const Vector3& vector) :x(vector.x), y(vector.y), z(vector.z)
	{
	}
	static Vector3 lerp(const Vector3& start, const Vector3& end, float delta) {
		Vector3 v;
		v.x = start.x * (1 - delta) + end.x * delta;
		v.y = start.y * (1 - delta) + end.y * delta;
		v.z = start.z * (1 - delta) + end.z * delta;
		return v;
	}
	static float distance(const Vector3& start, const Vector3& end) {
		return  ((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y) + (end.z - start.z) * (end.z - start.z));
	}
	Vector3 operator *(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}
	Vector3 operator /(float num)
	{
		return Vector3(x / num, y / num, z / num);
	}
	Vector3 operator +(Vector3 vector)
	{
		return Vector3(x + vector.x, y +  vector.y, z + vector.z);
	}
	Vector3 operator +=(Vector3 vector)
	{
		return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}
	Vector3 operator -(Vector3 vector)
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}
	Vector3 operator -=(Vector3 vector)
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}
	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}
	float length()
	{
		return sqrt(float(x * x + y * y + z * z));
	}
	Vector3 normalized()
	{
		float l = length();
		if (l != 0)
		{
			return Vector3(x, y, z) / length();
		}
		return Vector3(x, y, z);
	}
	static Vector3 Cross(Vector3 a, Vector3 b)
	{
		return Vector3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
	}
	static float Dot(Vector3 a, Vector3 b)
	{
		return float(a.x * b.x + a.y * b.y + a.z * b.z);
	}
public:
	float x, y, z;
};