#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class LookAtObject : public Component
{
public:
	GameObject* object;
public:
	void Update()
	{
		gameobject->transform.rotation = Quaternion::LookAt(gameobject->transform.position, object->transform.position);
	}
};