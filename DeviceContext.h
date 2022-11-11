#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* m_system);
	void clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha);

	void clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha);
	void clearDepthStencil(const TexturePtr& depth_chain);

	void setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_chain);

	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTrianglesList(UINT  index_count, UINT start_vertex_index, UINT start);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setPixelShader(PixelShaderPtr pixel_shader);
	void setVertexShader(VertexShaderPtr vertex_shader);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void setConsantBufffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer);
	void setConsantBufffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer);

	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_system = nullptr;
private:
	friend class ConstantBuffer;
};