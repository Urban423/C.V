#pragma once

class TimeSystem
{
public:
	float delta_time;
private:
	long old_delta;
	long new_delta;
public:
	TimeSystem() {};

	void TimeUpdate()
	{
		old_delta = new_delta;
		new_delta = long(::GetTickCount64());
		delta_time = (old_delta) ? ((new_delta - old_delta) / 1000.0f) : 0;
	}

	~TimeSystem() {};
};