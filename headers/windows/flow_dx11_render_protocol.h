#pragma once
#include "flow_dx11_pixel_shader.h"
#include "flow_dx11_vertex_shader.h"
#include "flow_dxfwd.h"
#include <string>

namespace flow::win32
{
    class DX11RenderProtocol
    {
        static DX11RenderProtocol* current;

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

    public:
        std::string _windowTitle = "DX11 Window";
        UINT _renderWidth = 1920;
        UINT _renderHeight = 1080;
        bool _vSync;
        DX11RenderProtocol(const UINT& windowWidth, const UINT& windowHeight, const std::string& windowTitle,
                           const bool& vSync)
            : _windowTitle(windowTitle), _renderWidth(windowWidth), _renderHeight(windowHeight), _vSync(vSync)
        {
            current = this;
        };
        HRESULT Init(HWND* hwnd, HINSTANCE* hInstance);
        HRESULT InitD3D(HWND hwnd);
        HRESULT CreateResources();
        HRESULT ApplyChanges();
        void ClearState();
        void CleanUp();
        void ClearFrame();
        void EndFrame();

        ID3D11DeviceContext* GetContext() const;
        ID3D11Device* GetDevice() const;
        IDXGISwapChain* GetSwapChain() const;

    private:
        // WINAPI Window Protocol
        static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
