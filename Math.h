#pragma once

class Math
{
public:
	static double pi;
public:
    static float lerp(float start, float end, float delta)
    {
        return start * (1.0f - delta) + end * delta;
    }
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
    static int convert16to10(char a[10])
    {
        int i, s, k, p;
        s = 0;
        p = strlen(a) - 1;
        for (i = 0; a[i] != '\0'; i++)
        {
            switch (toupper(a[i]))
            {
            case 'A': k = 10; break;
            case 'B': k = 11; break;
            case 'C': k = 12; break;
            case 'D': k = 13; break;
            case 'E': k = 14; break;
            case 'F': k = 15; break;
            case '1': k = 1; break;
            case '2': k = 2; break;
            case '3': k = 3; break;
            case '4': k = 4; break;
            case '5': k = 5; break;
            case '6': k = 6; break;
            case '7': k = 7; break;
            case '8': k = 8; break;
            case '9': k = 9; break;
            case '0': k = 0; break;
            }
            s = s + k * pow(16, p);
            p -= 1;
        }

        return s;
    }
};