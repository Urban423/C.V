#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <memory>
#include "Math.h"
#include <cmath>

class Quaternion
{
public:
	Quaternion() :x(0), y(0), z(0), w(0)
	{

	};
	Quaternion(float x1, float y1, float z1, float w1)
	{
		x = x1;
		y = y1;
		z = z1;
		w = w1;
	};
	Quaternion(Vector3 euler)
	{
		euler = euler * (Math::PI() / 180);
		double cy = cos(euler.z * 0.5);
		double sy = sin(euler.z * 0.5);

		double cp = cos(euler.y * 0.5);
		double sp = sin(euler.y * 0.5);

		double cr = cos(euler.x * 0.5);
		double sr = sin(euler.x * 0.5);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	};	
	static Vector3 QuanRotation(Vector3 v, Quaternion q)
	{
		float u0 = v.x * q.x + v.y * q.y + v.z * q.z;
		float u1 = v.x * q.w - v.y * q.z + v.z * q.y;
		float u2 = v.x * q.z + v.y * q.w - v.z * q.x;
		float u3 = -v.x * q.y + v.y * q.x + v.z * q.w;
		Quaternion M = Quaternion(u1, u2, u3, u0);

		Vector3 resultVector;
		resultVector.x = q.w * M.x + q.x * M.w + q.y * M.z - q.z * M.y;
		resultVector.y = q.w * M.y - q.x * M.z + q.y * M.w + q.z * M.x;
		resultVector.z = q.w * M.z + q.x * M.y - q.y * M.x + q.z * M.w;

		return resultVector;
	};
	static Vector3 QuarToVector3(Quaternion q)
	{
		Vector3 angles;
		double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);
		double sinp = 2 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1)
		{
			angles.y = std::copysign(3.14 / 2, sinp);
		}
		else
		{
			angles.y = std::asin(sinp);
		}
		double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return angles * (180 / Math::PI());
	}
	Vector4 getQuaternion()
	{
		return Vector4(x, y, z, w);
	}
public:
	float x, y, z, w;
};