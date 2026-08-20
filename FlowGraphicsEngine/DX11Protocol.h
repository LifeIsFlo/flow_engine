#pragma once
#include <d3d11.h>
#include <iostream>
#include "FWD.h"
#include "Color.h"
#include <vector>
#include <wrl/client.h>

#include "Camera.h"

namespace FlowEngineDX11
{
	class DX11Protocol
	{
		static DX11Protocol* current;
		std::string _windowTitle;
		std::vector<Renderer*> renderers;
		std::vector<Light*> lights;
		UINT _renderHeight;
		UINT _renderWidth;
		ID3D11RenderTargetView* g_renderTargetView = nullptr;
		ID3D11DepthStencilView* g_depthStencilView = nullptr;
		ID3D11Texture2D* g_depthStencilBuffer = nullptr; 
		ID3D11RasterizerState* g_rasterizerState = nullptr;
		ID3D11DepthStencilState* g_depthStencilState = nullptr;
		ID3D11BlendState* g_blendState = nullptr;
		ID3D11DeviceContext* g_context = nullptr;
		ID3D11Texture2D* g_backBuffer = nullptr;
		Camera* mainCamera = nullptr;
		IDXGISwapChain* g_swapChain = nullptr;
		Shader* shader = nullptr;
		ID3D11Device* g_device = nullptr;
		ID3D11SamplerState* texture2dSampler = nullptr;
		bool _fullScreen;
		bool _vSync;
	public:
		DX11Protocol(const UINT& windowHeight, const UINT& windowWidth, const std::string& windowTitle, const bool& fullScreen, const bool& vSync);
		bool Init();
		HRESULT CreateResources();
		void Render();
		HRESULT ApplyChanges();
		Light* AddLight(Light* light);
		Renderer* AddRenderer(Renderer* renderer);
		void RemoveLight(Light* light);
		void RemoveRenderer(Renderer* renderer);
		Camera* SetCamera(Camera* camera)
		{
			this->mainCamera = camera;
			return mainCamera;
		}
		bool isMainCamera(const Camera* camera) const
		{
			return mainCamera == camera;
		}
		void CleanUp();
		void BeginFrame(Color color);
		void EndFrame();
		void FlushUnusedData();
	private:
		static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HRESULT InitD3D(HWND hwnd);
	};
}
