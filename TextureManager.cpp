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

TexturePtr TextureManager::createTexture(const Rect& rect, Texture::Type type)
{
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(rect, type);
	}
	catch (...) {}
	TexturePtr res(tex);
	return res;
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	return new Texture(file_path);
}