#pragma once
#include "FWD.h"

namespace FlowEngineDX11
{
	class DX11Protocol
	{
		static DX11Protocol* current;
	public:
		std::string _windowTitle;
		UINT _renderHeight;
		UINT _renderWidth;
	private:
		ID3D11DeviceContext* g_context = nullptr;
		IDXGISwapChain* g_swapChain = nullptr;
		ID3D11Texture2D* g_backBuffer = nullptr;
		ID3D11RenderTargetView* g_renderTargetView = nullptr;
		ID3D11DepthStencilView* g_depthStencilView = nullptr;
		ID3D11Texture2D* g_depthStencilBuffer = nullptr;
		ID3D11RasterizerState* g_rasterizerState = nullptr;
		ID3D11DepthStencilState* g_depthStencilState = nullptr;
		ID3D11BlendState* g_blendState = nullptr;
		ID3D11SamplerState* texture2dSampler = nullptr;
		ID3D11Device* g_device = nullptr;
		VertexShader* vertexShader = nullptr;
		PixelShader* pixelShader = nullptr;
	public:
		bool _vSync;
		DX11Protocol(const UINT& windowHeight, const UINT& windowWidth, const std::string& windowTitle, const bool& vSync)
			: _renderHeight(windowHeight), _renderWidth(windowWidth), _windowTitle(windowTitle), _vSync(vSync) 
		{
			current = this;
		};
		HRESULT Init(HWND* hwnd, HINSTANCE* hInstance);
		HRESULT InitD3D(HWND hwnd);
		HRESULT CreateResources();
		HRESULT ApplyChanges();
		void ClearState();
		void CleanUp();
		void PrepareShadowPass();
		void PrepareMainPass();
		void EndFrame();

		ID3D11DeviceContext* GetContext() const;
		ID3D11Device* GetDevice() const;
		IDXGISwapChain* GetSwapChain() const;
	private:
		// Window Protocol
		static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
