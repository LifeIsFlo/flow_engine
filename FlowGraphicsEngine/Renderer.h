#pragma once
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "DX11Protocol.h"

namespace FlowEngineDX11 {
	class Renderer : public Component
	{
		ComPtr<ID3D11Texture2D> texture2d;
		ComPtr<ID3D11ShaderResourceView> texture2dSRV;
		ComPtr<ID3D11Buffer> indexBuffer;
		ComPtr<ID3D11Buffer> vertexBuffer;
		virtual void OnRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device);
	public:
		void Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device);
		HRESULT CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device);
	private:
		HRESULT CreateTexture(ID3D11Device* g_device);
		HRESULT UploadTexture(ID3D11DeviceContext* g_context);
		HRESULT CreateBuffers(ID3D11Device* g_device);
	public:
		float GetWidth()
		{
			return transform->scale.x;
		}
		float GetHeight()
		{
			return transform->scale.y;
		}
		std::vector<Color> GetColorData()
		{
			std::vector<Color> colors;
			colors.resize(GetHeight() * GetWidth());

			for (int y = 0; y < GetHeight(); y++) {
				for (int x = 0; x < GetWidth(); x++)
				{
					int i = y * GetWidth() + x;
					colors[i] = Color(255, 255, 255, 255);
				}
			}

			return colors;
		}
		ObjectRegistry GetRegistry() override;
	};
}
