#pragma once

class Component
{
public:
	bool enabled = true;
public:
	Component() {};
	virtual void Update() {};
	~Component() {};
};

