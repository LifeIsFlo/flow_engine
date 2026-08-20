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
	public:
		UINT artificialTextureWidth = 10;
		UINT artificialTextureHeight = 10;
		Sprite(UINT artificialWidth, UINT artificialHeight);
		~Sprite();
		HRESULT CreateTexture(ID3D11Device* g_device);
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
			return artificialTextureWidth;
		}

		UINT GetHeight()
		{
			if (texture2d) {
				D3D11_TEXTURE2D_DESC desc;
				texture2d->GetDesc(&desc);
				return desc.Height;
			}
			return artificialTextureHeight;
		}

		void GetArtificialColorData(std::vector<Color>* colors);
	};
}
