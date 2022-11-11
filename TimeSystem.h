#pragma once

class Time
{
public:
	static float delta_time;
	static float timescale;
private:
	static long old_delta;
	static long new_delta;
public:
	static void CreateTime()
	{
		old_delta = long(::GetTickCount64());
		new_delta = old_delta;
		timescale = 1;
	}
	static void TimeUpdate()
	{
		old_delta = new_delta;
		new_delta = long(::GetTickCount64());
		delta_time = (old_delta) ? ((new_delta - old_delta) / 1000.0f) * timescale : 0;
	}
};