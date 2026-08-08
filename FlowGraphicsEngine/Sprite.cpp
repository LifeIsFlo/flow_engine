#include "Sprite.h"
#include "Color.h"
#include "Vector2.h"
#include <d3d11.h>
#include "FWD.h"

FlowEngineDX11::Sprite::Sprite(UINT artificialWidth, UINT artificialHeight)
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
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = g_device->CreateTexture2D(&desc, nullptr, &texture2d);
	if (FAILED(hr)) return hr;

	hr = g_device->CreateShaderResourceView(texture2d, nullptr, &texture2dSRV);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT FlowEngineDX11::Sprite::UploadTexture(ID3D11DeviceContext* g_context)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = g_context->Map(
		texture2d,
		0,
		D3D11_MAP_WRITE_DISCARD, // Discards last frame's data so GPU doesn't stall
		0,
		&mappedResource
	);

	if (SUCCEEDED(hr)) {
		auto colors = GetArtificialColorData();
		BYTE* gpuMemory = static_cast<BYTE*>(mappedResource.pData);
		BYTE* cpuMemory = reinterpret_cast<BYTE*>(colors.data());

		size_t byteWidthCount = GetWidth() * sizeof(Color);

		// Row-by-row copy handles potential GPU memory alignment padding securely
		for (UINT row = 0; row < GetHeight(); ++row) {
			memcpy(
				gpuMemory + (row * mappedResource.RowPitch),
				cpuMemory + (row * byteWidthCount),
				byteWidthCount
			);
		}

		g_context->Unmap(texture2d, 0);
	}
	else
	{
		return hr;
	}

	return S_OK;
}

std::vector<FlowEngineDX11::Color> FlowEngineDX11::Sprite::GetArtificialColorData()
{
	std::vector<Color> colors;
	colors.resize(artificialTextureHeight * artificialTextureWidth);

	for (int i = 0; i < colors.size(); i++)
	{
		int y = std::floor(i / artificialTextureWidth);
		int x = i - y * artificialTextureWidth;
		Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };
		Vector2 center = { artificialTextureWidth / 2.0f, artificialTextureHeight / 2.0f };
		float distance = Vector2::Distance(center, pos);

		if (distance < 100 && distance > 98)
		{
			colors[i] = Color(255, 0, 0, 255);
		}
	}

	return colors;
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
