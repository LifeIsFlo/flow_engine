#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {
	struct ShadowMap
	{
	private:
		ID3D11Texture2D* cubeMap = nullptr;
		ID3D11Texture2D* stagedCubeMap = nullptr;
		ID3D11DepthStencilView* depthViews[6] = {};
		ID3D11ShaderResourceView* depthSRV = nullptr;
		UINT m_resolution;

		D3D11_VIEWPORT m_viewport = {};

		ShadowMap(UINT resolution) :
			m_resolution(resolution) {}
	public:
		~ShadowMap();
		static HRESULT Create(const DX11Protocol* dx11_protocol, const UINT& resolution, ShadowMap** shadowMap);
		void Save(const DX11Protocol* dx11_protocol);
		UINT GetResolution() const;
		D3D11_VIEWPORT* GetViewport();
		ID3D11Texture2D* GetCubeMap() const;
		ID3D11Texture2D** GetCubeMapAddress();
		ID3D11DepthStencilView* GetDSV(const int& index) const;
		ID3D11DepthStencilView** GetDSVAddress(const int& index);
		ID3D11ShaderResourceView* GetDepthSRV() const;
		ID3D11ShaderResourceView** GetDepthSRVAddress();
	};
}