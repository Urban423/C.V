#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <memory>
#include <cmath>

const float pi = 3.1416f;

class Quaternion
{
public:
	Quaternion()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
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
		euler = euler * 0.017453f;
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
	Vector3 QuanRotation(const Vector3& v)
	{
		float u0 = v.x * x + v.y * y + v.z * z;
		float u1 = v.x * w - v.y * z + v.z * y;
		float u2 = v.x * z + v.y * w - v.z * x;
		float u3 = -v.x * y + v.y * x + v.z * w;
		Quaternion M = Quaternion(u1, u2, u3, u0);

		return Vector3(
			w * M.x + x * M.w + y * M.z - z * M.y,
			w * M.y - x * M.z + y * M.w + z * M.x,
			w * M.z + x * M.y - y * M.x + z * M.w);
	};
	static Vector3 QuanRotation2(const Vector3& v, const Quaternion& q)
	{
		float u0 = v.x * q.x + v.y * q.y + v.z * q.z;
		float u1 = v.x * q.w - v.y * q.z + v.z * q.y;
		float u2 = v.x * q.z + v.y * q.w - v.z * q.x;
		float u3 = -v.x * q.y + v.y * q.x + v.z * q.w;
		Quaternion M = Quaternion(u1, u2, u3, u0);

		return Vector3(
			q.w * M.x + q.x * M.w + q.y * M.z - q.z * M.y,
			q.w * M.y - q.x * M.z + q.y * M.w + q.z * M.x,
			q.w * M.z + q.x * M.y - q.y * M.x + q.z * M.w);
	};
	Vector3 QuarToVector3()
	{
		Vector3 angles;
		double sinr_cosp = 2 * double(w * x + y * z);
		double cosr_cosp = 1 - 2 * double(x * x + y * y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);
		double sinp = 2 * double(w * y - z * x);
		if (std::abs(sinp) >= 1)
		{
			angles.y = std::copysign(3.14 / 2, sinp);
		}
		else
		{
			angles.y = std::asin(sinp);
		}
		double siny_cosp = 2 * double(w * z + x * y);
		double cosy_cosp = 1 - 2 * double(y * y + z * z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return angles * (180.0f / pi);
	}
	static Quaternion Slerp(Quaternion a, Quaternion b, double delta)
	{
		Quaternion qm;

		double cosHalfTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		if (abs(cosHalfTheta) >= 1.0)
		{
			qm.w = a.w;
			qm.x = a.x;
			qm.y = a.y; 
			qm.z = a.z;
			return qm;
		}
		double halfTheta = acos(cosHalfTheta);
		double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
		if (fabs(sinHalfTheta) < 0.001)
		{ 
			qm.w = (a.w * 0.5 + b.w * 0.5);
			qm.x = (a.x * 0.5 + b.x * 0.5);
			qm.y = (a.y * 0.5 + b.y * 0.5);
			qm.z = (a.z * 0.5 + b.z * 0.5);
			return qm;
		}
		double ratioA = sin((1 - delta) * halfTheta) / sinHalfTheta;
		double ratioB = sin(delta * halfTheta) / sinHalfTheta;
		qm.w = (a.w * ratioA + b.w * ratioB);
		qm.x = (a.x * ratioA + b.x * ratioB);
		qm.y = (a.y * ratioA + b.y * ratioB);
		qm.z = (a.z * ratioA + b.z * ratioB);
		return qm;
	}


	static Quaternion LookAt(Vector3 sourcePoint, Vector3 destPoint)
	{
		Vector3 forwardVector = Vector3::Normalize(destPoint - sourcePoint);

		float dot = Vector3::Dot(Vector3(0, 0, -1), forwardVector);

		if (abs(dot - (-1.0f)) < 0.000001f)
		{
			return Quaternion(0, 1, 0, 3.1415926535897932f);
		}
		if (abs(dot - (1.0f)) < 0.000001f)
		{
			return Quaternion();
		}

		float rotAngle = (float)acos(dot);
		Vector3 rotAxis = Vector3::Cross(Vector3(0, 0, -1), forwardVector);
		rotAxis = Vector3::Normalize(rotAxis);
		return CreateFromAxisAngle(rotAxis, rotAngle);
	}
	static Quaternion CreateFromAxisAngle(Vector3 axis, float angle)
	{
		float halfAngle = angle * .5f;
		float s = (float)sin(halfAngle);
		Quaternion q;
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = (float)cos(halfAngle);
		return q;
	}

	static Quaternion AxisAngle(Vector3 vector, float angle)
	{
		angle *= 0.017453f;
		double s = sin(angle / 2);
		return Quaternion(
			vector.x * s,
			vector.y * s,
			vector.z* s,
			cos(angle / 2));
	}

	static Quaternion Conjugate(Quaternion q)
	{
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

	Quaternion operator *(Quaternion q)
	{
		return Quaternion(
		x * q.w +y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		x * q.y - y * q.x + z * q.w + w * q.z,
		-x * q.x - y * q.y - z * q.z + w * q.w);
	}

	Quaternion operator +(Quaternion quaternion)
	{
		return Quaternion(x + quaternion.x, y + quaternion.y, z + quaternion.z, w + quaternion.w);
	}

	Vector4 getQuaternion()
	{
		return Vector4(x, y, z, w);
	}
public:
	float x, y, z, w;
};