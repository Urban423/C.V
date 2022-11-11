#include "Material.h"
#include "GraphicsEngine.h"
#include <exception>

Material::Material(const wchar_t* fullpath)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(fullpath, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_vertex_shader) throw std::exception("Material not created successfully");

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(fullpath, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_pixel_shader) throw std::exception("Material not created successfully");
}

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertex_shader_path, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_vertex_shader) throw std::exception("Material not created successfully");

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixel_shader_path, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_pixel_shader) throw std::exception("Material not created successfully");
}

Material::Material(const MaterialPtr& mat)
{
	m_vertex_shader = mat->m_vertex_shader;
	m_pixel_shader = mat->m_pixel_shader;
}

Material::~Material()
{
}

void Material::addTexture(const TexturePtr& texture)
{
	m_vec_tex.push_back(texture);
}

void Material::addTexture(const char* texture)
{
	TexturePtr founded_texture;
	m_vec_tex.push_back(founded_texture);
}

void Material::setTexture(const TexturePtr& texture, unsigned int index)
{
	if (index >= this->m_vec_tex.size())
	{
		addTexture(texture);
	}
	else
	{
		m_vec_tex[index] = texture;
	}
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_vec_tex.size()) return;
	m_vec_tex.erase(m_vec_tex.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
	{
		m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
	}
	else {
		m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
	}
}

void Material::setCullMode(CULL_MODE mode)
{
	m_cull_mode = mode;
}

CULL_MODE Material::getCullMode()
{
	return m_cull_mode;
}
