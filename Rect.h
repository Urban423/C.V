#pragma once
#include <Windows.h>

class Rect
{
public:
	Rect()
	{
		width = 0;
		height = 0;
		left = 0;
		top = 0;
	};
	Rect(int n_width, int n_height)
	{
		width = n_width;
		height = n_height;
	};
	Rect(int n_left, int n_top, int n_width, int n_height)
	{
		width = n_width;
		height = n_height;
		left = n_left;
		top = n_top;
	};
	Rect(RECT rect)
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		left = rect.left;
		top = rect.top;
	};
	Rect(const Rect& rect)
	{
		width = rect.width;
		height = rect.height;
		left = rect.left;
		top = rect.top;
	};
	~Rect() {};
public:
	int width;
	int height;
	int left;
	int top;
};