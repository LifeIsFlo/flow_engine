#include "Sprite.h"
#include "Color.h"
#include "Vector2.h"
#include <d3d11.h>
#include <iostream>
#include "DX11Protocol.h"
#include "FWD.h"

namespace FlowEngineDX11 
{
	HRESULT FlowEngineDX11::Sprite::CreateTexture(const DX11Protocol* dx11_protocol)
	{
		auto g_device = dx11_protocol->GetDevice();
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = _width;
		desc.Height = _height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource_data = { _colorData.data(), _width * sizeof(Color), 0 };

		HRESULT hr = g_device->CreateTexture2D(&desc, &subresource_data, &texture2d);
		if (FAILED(hr)) return hr;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = g_device->CreateShaderResourceView(texture2d, &srvDesc, &texture2dSRV);
		if (FAILED(hr)) return hr;

		_colorData.clear();
		_colorData.shrink_to_fit();

		return S_OK;
	}

	FlowEngineDX11::Sprite::~Sprite()
	{
		if (texture2d) {
			texture2d->Release();
			texture2d = nullptr;
		}
		if (texture2dSRV) {
			texture2dSRV->Release();
			texture2dSRV = nullptr;
		}
	}
}
