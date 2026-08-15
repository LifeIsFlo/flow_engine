#include "Sprite.h"
#include "Color.h"
#include "Vector2.h"
#include <d3d11.h>
#include <iostream>

#include "FWD.h"

FlowEngineDX11::Sprite::Sprite(UINT artificialWidth, UINT artificialHeight, std::string assetLocation) : AssetItem(assetLocation)
{
	this->artificialTextureWidth = artificialWidth;
	this->artificialTextureHeight = artificialHeight;
}

HRESULT FlowEngineDX11::Sprite::CreateTexture(ID3D11Device* g_device)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = artificialTextureWidth;
	desc.Height = artificialTextureHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	std::vector<Color> colors;
	GetArtificialColorData(&colors);
	D3D11_SUBRESOURCE_DATA subresource_data = { colors.data(), artificialTextureWidth * sizeof(Color), 0};

	HRESULT hr = g_device->CreateTexture2D(&desc, &subresource_data, &texture2d);
	if (FAILED(hr)) return hr;

	hr = g_device->CreateShaderResourceView(texture2d, nullptr, &texture2dSRV);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void FlowEngineDX11::Sprite::GetArtificialColorData(std::vector<Color>* colors)
{
	colors->resize(artificialTextureHeight * artificialTextureWidth);

	for (int i = 0; i < colors->size(); i++)
	{
		int y = std::floor(i / artificialTextureWidth);
		int x = i - y * artificialTextureWidth;
		Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };
		Vector2 center = { artificialTextureWidth / 2.0f, artificialTextureHeight / 2.0f };
		float distance = Vector2::Distance(center, pos);

		if (distance < center.x && distance > center.x / 1.25)
		{
			(*colors)[i] = Color(255, 0, 0, 255);
		}
		else
		{
			(*colors)[i] = Color(0, 0, 0, 100);
		}
	}
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
