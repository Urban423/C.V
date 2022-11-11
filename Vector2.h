#pragma once

class Vector2
{
public:
	Vector2() :x(0), y(0)
	{
	}
	Vector2(float x, float y) :x(x), y(y)
	{
	}
	Vector2(const Vector2& vector) :x(vector.x), y(vector.y)
	{
	}
	static Vector2 lerp(const Vector2& start, const Vector2& end, float delta) {
		Vector2 v;
		v.x = start.x * (1 - delta) + end.x * delta;
		v.y = start.y * (1 - delta) + end.y * delta;
		return v;
	}
	Vector2 operator *(float num)
	{
		return Vector2(x * num, y * num);
	}
	Vector2 operator +(Vector2 vector)
	{
		return Vector2(x + vector.x, y + vector.y);
	}
	Vector2 operator -(Vector2 vector) const
	{
		return Vector2(x - vector.x, y - vector.y);
	}

public:
	float x, y;
};