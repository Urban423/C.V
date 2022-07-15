#include "Texture.h"
#include "GraphicsEngine.h"
#include <DirectXTex.h>
#include <exception>

Texture::Texture(const wchar_t* fullpath) : Resource(fullpath)
{
	DirectX::ScratchImage mipChain;
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(fullpath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

	if (SUCCEEDED(res))
	{
		res = DirectX::GenerateMipMaps(image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), 0, 0, mipChain);

		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, mipChain.GetImages(),
			mipChain.GetImageCount(), mipChain.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = -1;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res))
		{
			throw std::exception("Texture not created successfully");
		}
		res = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shader_res_view);
		if (FAILED(res))
		{
			throw std::exception("Texture not created successfully");
		}
	}
	else
	{
		throw std::exception("Texture not created succsefully");
	}
}

Texture::~Texture()
{
	m_shader_res_view->Release();
	m_texture->Release();
}

