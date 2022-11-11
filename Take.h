#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Take : public Component
{
public:
	bool e = false;
	GameObject* player;
public:
	void Do()
	{
		gameobject->GetComponent<Rigidbody>()->isKinematic = true;
		gameobject->GetComponent<BoxCollider>()->enabled = false;
		gameobject->transform.parent = player;
		gameobject->transform.localPosition = Vector3(1, -0.7f, 2);
		gameobject->transform.localRotation = Vector3(0, 0, 0);
	}
};