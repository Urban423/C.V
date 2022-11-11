#pragma once
#include "ResourceManager.h"
#include "Texture.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* file_path);
	TexturePtr createTexture(const Rect& rect, Texture::Type type);
private:
	TexturePtr* list;

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};

