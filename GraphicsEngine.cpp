#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...) {
		throw std::exception("Render Exngine not created succesfully");
	}
	try
	{
		m_texture_manager = new TextureManager();
	}
	catch (...) {
		throw std::exception("Texture Manager not created succesfully");
	}
	try
	{
		m_mesh_manager = new MeshManager();
	}
	catch (...) {
		throw std::exception("Mesh Manager not created succesfully");
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	m_render_system->compileVertexShader(L"Assets\\Shaders\\VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);

	m_mesh_layout_size = size_shader;
	m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_texture_manager;
	delete m_mesh_manager;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_texture_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

MaterialPtr GraphicsEngine::createMaterialFromFile(const wchar_t* fullpath)
{
	return std::make_shared<Material>(fullpath);
}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& material)
{
	return std::make_shared<Material>(material);
}

void GraphicsEngine::setMaterial(const MaterialPtr& mat)
{
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(mat->m_cull_mode == CULL_MODE_FRONT);
	if (mat->m_constant_buffer)
	{
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConsantBufffer(mat->m_vertex_shader, mat->m_constant_buffer);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConsantBufffer(mat->m_pixel_shader, mat->m_constant_buffer);
	}
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mat->m_vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mat->m_pixel_shader);
	if (mat->m_vec_tex.size())
	{
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(mat->m_pixel_shader, &mat->m_vec_tex[0], mat->m_vec_tex.size());
	}
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("Graphics Engine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}
