#pragma once

class Math
{
public:
	static double PI() { return 3.14f; }

	static float clamp(float x, float upper, float lower) 
	{
		if (x < lower)
		{
			x = lower;
		}
		if (x > upper)
		{
			x = upper;
		}
		return x;
	}
};