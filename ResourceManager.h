#pragma once
#include <map>
#include <string>
#include "Prerequisites.h"
#include "Resource.h"

class ResourceManager
{
public:
	ResourceManager();
	ResourcePtr createResourceFromFile(const wchar_t* file_path);
	virtual ~ResourceManager();
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0; 
protected:
	std::map<std::wstring, ResourcePtr> m_map_resources;
};

