#include "BoxCollider.h"
#include <vector>
#include <algorithm>

void BoxCollider::GetAxis(Vector3 a[], Vector3 b[], Vector3* result)
{
    Vector3 A;
    Vector3 B;

    for (int i = 1; i < 4; i++)
    {
        A = a[i] - a[0];
        B = a[(i + 1) % 3 + 1] - a[0];
        result[i - 1] = Vector3::Cross(A, B).normalized();
    }
    for (int i = 1; i < 4; i++)
    {
        A = b[i] - b[0];
        B = b[(i + 1) % 3 + 1] - b[0];
        result[2 + i] = Vector3::Cross(A, B).normalized();
    }
    int n = 6;
    for (int i = 1; i < 4; i++)
    {
        A = a[i] - a[0];
        for (int j = 1; j < 4; j++)
        {
            B = b[j] - b[0];
            result[n] = Vector3::Cross(A, B).normalized();
            n += 1;
        }
    }
}

Vector3 BoxCollider::onCollisonEnter(GameObject first, GameObject second)
{
    Vector3* list = GetPoint(first.GetComponent<BoxCollider>(),  first.transform.rotation, first.transform.position);
    Vector3* list1 = GetPoint(second.GetComponent<BoxCollider>(), second.transform.rotation, second.transform.position);

    Vector3 axis[15] = {};
    GetAxis(list, list1, axis);

    return IntersectionOfProj(list, list1, axis);
}

float BoxCollider::ProjVector3(Vector3 v, Vector3 a)
{
	return a.Dot(v, a) / a.length();
}

Vector3 BoxCollider::IntersectionOfProj(Vector3 a[], Vector3 b[], Vector3 Axis[])
{
    Vector3 norm = Vector3(10000, 10000, 10000);
	for (int j = 0; j < 15; j++)
	{
        Vector2 a1 = ProjAxis(a, Axis[j]);
        Vector2 b1 = ProjAxis(b, Axis[j]);

        float points[4] = { a1.y, a1.x, b1.y, b1.x };

        float sum = (b1.x - b1.y) + (a1.x - a1.y);
        float len = std::abs(points[3] - points[0]);

        if (sum <= len)
        {
            return Vector3(0, 0, 0);
        }
        float dl = std::abs(points[2] - points[1]);
        if (dl < norm.length())
        {
            norm = Axis[j] * dl;
            if (points[0] != a1.y)
            {
                norm = -norm;
            }
        }
	}
	return norm;
}

Vector2 BoxCollider::ProjAxis(Vector3 points[], Vector3 Axis)
{
    Vector2 m = Vector2();
    m.x = ProjVector3(points[0], Axis);
    m.y = m.x;
    for (int i = 1; i < 8; i++)
    {
        float tmp = ProjVector3(points[i], Axis);
        if (tmp > m.x)
        {
            m.x = tmp;
        }

        if (tmp < m.y)
        {
            m.y = tmp;
        }
    }
    return m;
}

Vector3* BoxCollider::GetPoint(BoxCollider* box, Quaternion qu, Vector3 worldpos)
{
    Vector3 point[8] = {};
    point[0] = worldpos + box->Center - box->size / 2;
    point[1] = point[0] + Vector3(box->size.x, 0, 0);
    point[2] = point[0] + Vector3(0, box->size.y, 0);
    point[3] = point[0] + Vector3(0, 0, box->size.z);
    point[4] = worldpos + box->Center + box->size / 2;
    point[5] = point[4] - Vector3(box->size.x, 0, 0);
    point[6] = point[4] - Vector3(0, box->size.y, 0);
    point[7] = point[4] - Vector3(0, 0, box->size.z);
    for (int i = 0; i < 8; i++)
    {
        point[i] = point[i] - worldpos - box->Center;
        point[i] = qu.QuanRotation(point[i]);
        point[i] = point[i] + worldpos + box->Center;
    }
    return point;
}