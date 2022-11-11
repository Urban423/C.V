#include "Physics.h"

bool Physics::Raycast(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit)
{
    std::list<GameObject>* list = GameObjectManager::get()->ListOfGameObjects();
    std::list<GameObject>::iterator it = list->begin();
    bool ret = false;
    hit->distance = 111111;
    for (int i = 0; i < GameObjectManager::get()->NumberOfGameObjects(); i++)
    {
        if ((*it).GetComponent<BoxCollider>())
        {
            ret = boxIntersection(rayOrigin, rayVector, distance, hit, &(*it)) || ret;
        }
        else if ((*it).GetComponent<SphereCollider>())
        {
            ret = sphereIntersection(rayOrigin, rayVector, distance, hit, &(*it)) || ret;
        }
        std::advance(it, 1);
    }
	return ret;
}

bool Physics::boxIntersection(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit, GameObject* box)
{
    Vector3* list = BoxCollider::GetPoint(box->GetComponent<BoxCollider>(), box->transform.rotation, box->transform.position);
    bool ret = false;

    trinaglehelper(rayOrigin, rayVector, distance, 0, 1, 2, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 1, 2, 7, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 2, 5, 7, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 4, 5, 7, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 7, 4, 6, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 1, 6, 7, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 0, 2, 3, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 2, 5, 3, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 0, 1, 6, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 0, 3, 6, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 3, 4, 5, hit, list, ret, box);
    trinaglehelper(rayOrigin, rayVector, distance, 3, 4, 6, hit, list, ret, box);

    return ret;
}

bool Physics::sphereIntersection(Vector3 rayOrigin, Vector3 rayVector, float distance, RaycastHit* hit, GameObject* sphere)
{
    SphereCollider* spc = sphere->GetComponent<SphereCollider>();
    Vector3 center = spc->Center + sphere->transform.position;

    Vector3 collisionPoint;
    Vector3 v;
    Vector3 CA = rayOrigin - center;
    float rSquared = spc->radius * spc->radius;
    float vSquared;
    if (Vector3::Dot(CA, CA) <= rSquared)
    {
        collisionPoint = rayOrigin;
        
        float new_dist = (collisionPoint - rayOrigin).length();
        if (hit->distance > new_dist && new_dist <= distance)
        {
            hit->hitObject = sphere;
            hit->distance = new_dist;
            hit->normal = (collisionPoint - center).normalized();
            hit->point = collisionPoint;
            return true;
        }
    }
    else if (Vector3::Dot(CA, rayVector) <= 0)
    {
        v = CA - Vector3::Project(CA, rayVector);
        vSquared = Vector3::Dot(v, v);
        if (vSquared <= rSquared) 
        {
            collisionPoint = center + v - rayVector.normalized() * sqrt(rSquared - vSquared);
            
            float new_dist = (collisionPoint - rayOrigin).length();
            if (hit->distance > new_dist && new_dist <= distance)
            {
                hit->hitObject = sphere;
                hit->distance = new_dist;
                hit->normal = (collisionPoint - center).normalized();
                hit->point = collisionPoint;
                return true;
            }
        }
    }
    return false;
}

bool Physics::RayIntersectsTriangle(Vector3 rayOrigin, Vector3 rayVector, Vector3 vertex0, Vector3 vertex1, Vector3 vertex2, Vector3& outIntersectionPoint)
{
    const float EPSILON = 0.0000001;
    Vector3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = Vector3::Cross(rayVector, edge2);
    a = Vector3::Dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
    {
        return false;
    }
    f = 1.0 / a;
    s = rayOrigin - vertex0;
    u = f * Vector3::Dot(s, h);
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }
    q = Vector3::Cross(s, edge1);
    v = f * Vector3::Dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }
    float t = f * Vector3::Dot(q, edge2);
    if (t > EPSILON)
    {
        outIntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    return false;
}

void Physics::trinaglehelper(Vector3 rayOrigin, Vector3 rayVector, float distance, int i1, int i2, int i3, RaycastHit* hit, Vector3* list, bool& ret, GameObject* obj)
{
    Vector3 checker;
    if (RayIntersectsTriangle(rayOrigin, rayVector, list[i1], list[i2], list[i3], checker))
    {
        float new_dist = (checker - rayOrigin).length();
        if (hit->distance > new_dist && new_dist <= distance)
        {
            ret = true;
            hit->hitObject = obj;
            hit->distance = new_dist;
            hit->point = checker;
        }
    }
}

Physics::Physics()
{
	Gravity = Vector3(0, -9.81f, 0);
}

Physics::~Physics()
{
}
