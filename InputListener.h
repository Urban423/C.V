#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener() { }
	~InputListener() { }

	virtual void onKeyUp(int key) = 0;
};