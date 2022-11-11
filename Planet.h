#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Planet : public Component
{
public:
	GameObject* Mercury;
	GameObject* Venus;
	GameObject* Earth;
	GameObject* Mars;
	GameObject* Jupiter;
	GameObject* Saturn;
	GameObject* Uranus;
	GameObject* Neptune;
	GameObject* Centaurs;
	GameObject* sun;
	const double gravityConst = 6.6743 * pow(10, -8);
public:
	void Update()
	{
		if (Time::delta_time)
		{
			Vector3 gravityDirection = sun->transform.position - gameobject->transform.position;
			float gravityAcceleration = gravityConst * sun->GetComponent<Rigidbody>()->mass / Vector3::Distance(gameobject->transform.position, sun->transform.position);
			gameobject->GetComponent<Rigidbody>()->velocity = gravityDirection.normalized() * gravityAcceleration * Time::delta_time + gameobject->GetComponent<Rigidbody>()->velocity;
			gameobject->transform.position = gameobject->GetComponent<Rigidbody>()->velocity + gameobject->transform.position;
		}
	}
};