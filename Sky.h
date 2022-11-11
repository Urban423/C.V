#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Sky : public Component
{
public:
	GameObject* Cam;
public:
	void Update()
	{
		gameobject->transform.position = Cam->transform.position;
	}
};