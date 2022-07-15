#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
public:
	RenderSystem * getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();
public:
	MaterialPtr createMaterialFromFile(const wchar_t* fullpath);
	MaterialPtr createMaterial(const MaterialPtr& material);
	void setMaterial(const MaterialPtr& mat);
	void getVertexMeshLayoutShaderByteCodeAndSize(void ** byte_code, size_t * size);
public:
	static GraphicsEngine* get();
	static void create();
	static void release();
private:
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
	static GraphicsEngine* m_engine;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};