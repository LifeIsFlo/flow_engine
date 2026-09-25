#pragma once
#include <d3d11.h>
#include "Object.h"
#include "FWD.h"

namespace FlowEngineDX11
{
	class Sprite : public Object
	{
		ID3D11Texture2D* texture2d = nullptr;
		ID3D11ShaderResourceView* texture2dSRV = nullptr;
		std::vector<Color> _colorData = {};
		UINT _width = 0;
		UINT _height = 0;
	public:
		Sprite(std::vector<Color>& colorData, UINT width, UINT height) : _colorData(colorData), _height(height), _width(width) {}
		~Sprite();
		HRESULT CreateTexture(const DX11Protocol* dx11_protocol);
		ID3D11ShaderResourceView* GetTextureResource()
		{
			return texture2dSRV;
		}
		ID3D11ShaderResourceView** GetTextureResourceAddress()
		{
			return &texture2dSRV;
		}
		UINT GetWidth()
		{
			if (texture2d) {
				D3D11_TEXTURE2D_DESC desc;
				texture2d->GetDesc(&desc);
				return desc.Width;
			}
			return _width;
		}

		UINT GetHeight()
		{
			if (texture2d) {
				D3D11_TEXTURE2D_DESC desc;
				texture2d->GetDesc(&desc);
				return desc.Height;
			}
			return _height;
		}
	};
}
