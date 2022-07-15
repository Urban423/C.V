#pragma once
#include <string>

class Resource
{
public:
	Resource(const wchar_t* fullpath);
	virtual ~Resource();
protected:
	std::wstring m_full_path;
};

