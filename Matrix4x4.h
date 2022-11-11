#pragma once
#include <memory>
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class Matrix4x4
{
public:
	Matrix4x4()
	{

	}
	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}
	void SetScale(const Vector3& scale)
	{
		m_mat[0][0] = scale.x;
		m_mat[1][1] = scale.y;
		m_mat[2][2] = scale.z;
	}
	void setTranslation(const Vector3& translation)
	{
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}
	void SetRotationX(float x)
	{
		m_mat[1][1] = cos(x);
		m_mat[1][2] = sin(x);
		m_mat[2][1] = -sin(x);
		m_mat[2][2] = cos(x);
	}

	void SetRotationY(float y)
	{
		m_mat[0][0] = cos(y);
		m_mat[0][2] = -sin(y);
		m_mat[2][0] = sin(y);
		m_mat[2][2] = cos(y);
	}

	void SetRotationZ(float z)
	{
		m_mat[0][0] = cos(z);
		m_mat[0][1] = sin(z);
		m_mat[1][0] = -sin(z);
		m_mat[1][1] = cos(z);
	}

	float getDeterminant()
	{
		Vector4 minor, v1, v2, v3;
		float det;

		v1 = Vector4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4 v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = float(pow(-1.0f, i)) * v.x / det;
			out.m_mat[1][i] = float(pow(-1.0f, i)) * v.y / det;
			out.m_mat[2][i] = float(pow(-1.0f, i)) * v.z / det;
			out.m_mat[3][i] = float(pow(-1.0f, i)) * v.w / det;
		}

		this->setMatrix(out);
	}

	void operator *= (const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int i1 = 0; i1 < 4; i1++)
			{
				out.m_mat[i][i1] =
					m_mat[i][0] * matrix.m_mat[0][i1] + m_mat[i][1] * matrix.m_mat[1][i1] +
					m_mat[i][2] * matrix.m_mat[2][i1] + m_mat[i][3] * matrix.m_mat[3][i1];
			}
		}
		::memcpy(m_mat, out.m_mat, sizeof(float)*16);
	}

	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	Vector3 getZDirection()
	{
		return Vector3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}
	
	Vector3 getXDirection()
	{
		return Vector3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	Vector3 getYDirection()
	{
		return Vector3(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}

	Vector3 getTranslation()
	{
		return Vector3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	Vector3 operator *(Vector3 v)
	{
		Vector3 t;
		t.x = m_mat[0][0] * v.x + m_mat[0][1] * v.y + m_mat[0][2] * v.z;
		t.y = m_mat[1][0] * v.x + m_mat[1][1] * v.y + m_mat[1][2] * v.z;
		t.z = m_mat[2][0] * v.x + m_mat[2][1] * v.y + m_mat[2][2] * v.z;
		return t;
	}

	void QuaternionToMatrix(const Quaternion* quat)
	{
		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		x2 = quat->x + quat->x;
		y2 = quat->y + quat->y;
		z2 = quat->z + quat->z;

		xx = quat->x * x2; 
		xy = quat->x * y2;  
		xz = quat->x * z2;

		yy = quat->y * y2; 
		yz = quat->y * z2; 
		zz = quat->z * z2;

		wx = quat->w * x2;
		wy = quat->w * y2;  
		wz = quat->w * z2;

		m_mat[0][0] = 1 - (yy + zz);
		m_mat[0][1] = xy + wz;
		m_mat[0][2] = xz - wy;
		m_mat[1][0] = xy - wz; 
		m_mat[1][1] = 1.0f - (xx + zz);
		m_mat[1][2] = yz + wx;
		m_mat[2][0] = xz + wy;
		m_mat[2][1] = yz - wx;   
		m_mat[2][2] = 1.0f - (xx + yy);

		m_mat[0][3] = m_mat[1][3] = m_mat[2][3] = 0;
		m_mat[3][0] = m_mat[3][1] = m_mat[3][2] = 0;
		m_mat[3][3] = 1;
	}

	void setPersepciveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}
private:
	float m_mat[4][4] = {};
};