#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class GraphicsEngine;

class Material
{
public:
	Material(const wchar_t* fullpath);
	Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	Material(const MaterialPtr& mat);
	~Material();

	void addTexture(const TexturePtr& texture);
	void addTexture(const char* texture);
	void setTexture(const TexturePtr& texture, unsigned int index);
	void removeTexture(unsigned int index);

	void setData(void* data, unsigned int size);

	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();
private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<TexturePtr> m_vec_tex;
	CULL_MODE m_cull_mode = CULL_MODE_FRONT;
	friend class GraphicsEngine;
};

