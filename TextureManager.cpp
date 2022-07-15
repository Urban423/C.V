#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* fullpath)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(fullpath));
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	return new Texture(file_path);
}