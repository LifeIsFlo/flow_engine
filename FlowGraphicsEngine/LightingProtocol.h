#pragma once
#include <d3d11.h>
#include "FWD.h"

namespace FlowEngineDX11
{
	class LightingProtocol
	{
		std::vector<Light*> lights;
		std::vector<Renderer*> shadows;
		ID3D11Buffer* g_lightBuffer = nullptr;
		ID3D11ShaderResourceView* g_lightDataSRV = nullptr;
		ID3D11ShaderResourceView* g_shadowDataSRV = nullptr;
		ID3D11Texture2D* g_shadowDataCubeMaps = nullptr;
		VertexShader* ShadowVS = nullptr;
		ID3D11RasterizerState* g_shadowRasterizerState = nullptr;
		ID3D11BlendState* g_shadowBlendState = nullptr;
	public:
		void ShadowPass(FlowEngine* engine, const DX11Protocol* dx11_protocol);
		void MainPass(const DX11Protocol* dx11_protocol);
		HRESULT CreateResources(const DX11Protocol* dx11_protocol);
		Light* AddLight(Light* light);
		void RemoveLight(Light* light);
		Renderer* AddShadow(Renderer* renderer);
		void RemoveShadow(Renderer* renderer);
		std::vector<Renderer*> GetShadows() const;
		void ClearState();
		~LightingProtocol();
	};
}
