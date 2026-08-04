#pragma once
#include <d3d11.h>
#include <iostream>
#include "Vertex2D.h"
#include "FWD.h"
#include "Color.h"
#include <vector>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace FlowEngineDX11
{
	class DX11Protocol
	{
		static DX11Protocol* current;
		std::string _windowTitle;
		std::vector<Renderer*> renderers;
		D3D11_VIEWPORT viewport;
		UINT _windowHeight;
		UINT _windowWidth;
		ComPtr<ID3D11Texture2D> backBuffer;
		ComPtr<ID3D11Device>           g_device;
		ComPtr<ID3D11DeviceContext>    g_context;
		ComPtr<IDXGISwapChain>         g_swapChain;
		ComPtr<ID3D11RenderTargetView> g_renderTargetView;
		ComPtr<ID3D11VertexShader> VS;
		ComPtr<ID3D11PixelShader> PS;
		bool _fullScreen;
	public:
		DX11Protocol(const UINT& windowHeight, const UINT& windowWidth, const std::string& windowTitle, const bool& fullScreen);
		bool Init();
		void Render();
		void ApplyChanges();
		Renderer* AddRenderer(Renderer* renderer);
		void CleanUp();
		void Clear(const Color color);
	private:
		static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HRESULT InitD3D(HWND hwnd);
	};
}