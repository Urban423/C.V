#pragma once
#include "Resource.h"
#include "Rect.h"
#include <d3d11.h>

class Texture : public Resource
{
public:
	enum Type
	{
		Normal = 0,
		RenderTarget,
		DepthStencil
	};
public:
	Texture(const wchar_t* fullpath);
	Texture(const Rect& rect, Texture::Type type);
	~Texture();

	Rect getSize();
	Texture::Type getType();
private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;

	char* name;
	Texture::Type type = Texture::Type::Normal;
	Rect size;
private:
	friend class DeviceContext;
};

