#include "SphereCollider.h"
#include <iostream>

Vector3 SphereCollider::onCollisonEnter(GameObject first, GameObject second)
{
    Vector3* list = BoxCollider::GetPoint(first.GetComponent<BoxCollider>(), first.transform.rotation, first.transform.position);

    Vector3 dir;
    SphereCollider* spc = second.GetComponent<SphereCollider>();
    Vector3 center_of_sphere = second.transform.position + spc->Center;
    float dist = 1111111;
    for (int i = 0; i < 8; i++)
    {
        float temp = Vector3::DistanceMagnitude(list[i], center_of_sphere);
        if (dist > temp)
        {
            dist = temp;
            dir = center_of_sphere - list[i];
        }
    }
    dist = sqrt(dist);
    if (dist < spc->radius)
    {
        dir = dir.normalized() * (spc->radius - dist);
    }
    else {
        dir = Vector3(0, 0, 0);
    }
    return dir;
}
