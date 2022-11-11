#pragma once
#include "GameObject.h"

class MeshRenderer : public Component
{
public:
	int number = 0;
	int sizeMaterialNumbers = 0;
	int MaterialNumbers[16] = {};
public:
	MeshRenderer()
	{

	}

	void AddMaterial(int Number)
	{
		MaterialNumbers[sizeMaterialNumbers] = Number;
		sizeMaterialNumbers += 1;
	}

	~MeshRenderer()
	{

	}
};