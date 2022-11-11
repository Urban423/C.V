#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Rocket : public Component
{
public:
	float p1 = 33;
	float p2 = 44;

	Vector3 st;
	Vector3 pos = Vector3(50, 0, 0);
	Vector3 velocity = Vector3(0, 0.3f, 0);
	Vector3 normal;

	float r = 63;
	float start_alpha = 100;
	float velocity_0 = 0.3f;
	float h = 0;
	float G = 6.6743 * pow(10, -8);
	float time = 0;
	float mass = 30000000000;

	bool starte = false;
	bool step1 = true;
	Matrix4x4 m_world;
public:
	void start()
	{
		Quaternion l = Quaternion::LookAt(Vector3(), Vector3::Cross(gameobject->transform.localPosition, normal));
		Vector3 c = l.QuarToVector3();
		c.x *= -1;
		c.y *= -1;
		l = c;

		m_world.setIdentity();
		m_world.QuaternionToMatrix(&l);
		Matrix4x4 temp;
		temp.setIdentity();
		temp.QuaternionToMatrix(&l);
		temp.inverse();

		pos = temp * gameobject->transform.localPosition;

		velocity = Vector3(sin((90 - start_alpha) * 0.01745329251f) * velocity_0, sin(start_alpha * 0.01745329251f) * velocity_0, 0);
		std::cout << velocity.x << " " << velocity.y << " " << velocity.z << "\n";
	}
	void Update()
	{
		if (!starte)
		{
			if (normal.length() != 0)
			{
				start();
				starte = true;
			}
			return;
		}

		Vector3 gravity_dir = (Vector3(0, 0, 0) - pos).normalized();
		float gamma = acos(Vector3::Dot(velocity, gravity_dir) / velocity.length());
		float g = G * mass / Vector3::DistanceMagnitude(Vector3(), pos);

		if (Time::delta_time > 0)
		{
			if (gamma > 3.14f / 2 && step1)
			{
				velocity = velocity + gravity_dir * g * Time::delta_time;
				pos = pos + velocity;
				float delta = asin(abs(velocity.x / (sqrt(velocity.y * velocity.y + velocity.x * velocity.x))));
				float sindelta = sin(delta);
				float cosdelta = cos(delta);
				if (velocity.x < 0)
					sindelta = -sindelta;
				if (velocity.y < 0)
					cosdelta = -cosdelta;
				velocity = Vector3(sindelta * velocity_0, cosdelta * velocity_0, 0);
			}
			else 
			{
				step1 = false;
				velocity = velocity + (Vector3(0, 0, 0) - pos).normalized() * g / 10 * Time::delta_time;
				pos = pos + velocity;
			}


			gameobject->transform.localRotation = Vector3(p1, 0, 0);
			gameobject->transform.localPosition = m_world * pos;
			//std::cout << gameobject->transform.localPosition.x << " " << gameobject->transform.localPosition.y << ' ' << pos.z << "\n";
			time += Time::delta_time * 220;
		}
	}
};
