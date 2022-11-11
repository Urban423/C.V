#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Rotate: public Component
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
	Vector3 rotate;
public:

	void Update()
	{
		y += rotate.y * Time::delta_time * 10;
		if (y > 180)
		{
			y -= 360;
		}
		if (y < -180)
		{
			y += 360;
		}

		gameobject->transform.rotation = Vector3(0, y, 0);
	};
};