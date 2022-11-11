#pragma once

class Point
{
public:
	Point() :x(0), y(0)
	{
	}
	Point(int x, int y):x(x), y(y)
	{
	}
	Point(const Point & point) :x(point.x), y(point.y)
	{
	}
public:
	int x = 0, y = 0;
};