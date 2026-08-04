#include "Vector2.h"
#include "FWD.h"
#include "DX11Protocol.h"
#include "Renderer.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
// Link required DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
using Microsoft::WRL::ComPtr;

FlowEngineDX11::DX11Protocol* FlowEngineDX11::DX11Protocol::current = nullptr;

FlowEngineDX11::DX11Protocol::DX11Protocol(const UINT& windowHeight, const UINT& windowWidth, const std::string& windowTitle, const bool& fullScreen)
{
    _windowHeight = windowHeight;
    _windowWidth = windowWidth;
    _windowTitle = windowTitle;
    _fullScreen = fullScreen;

    viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(_windowWidth);
    viewport.Height = static_cast<FLOAT>(_windowHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    current = this;
}

LRESULT CALLBACK FlowEngineDX11::DX11Protocol::WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return current->WindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT FlowEngineDX11::DX11Protocol::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        // Handle window resizing safely if initialization is finished
        if (g_swapChain && wParam != SIZE_MINIMIZED) {
			std::cout << "resize!";
            _windowWidth = LOWORD(lParam);
            _windowHeight = HIWORD(lParam);

            ApplyChanges();
        }
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

HRESULT FlowEngineDX11::DX11Protocol::InitD3D(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;                                    // One back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Color format
    scd.BufferDesc.Width = _windowWidth;                    // Match client width
    scd.BufferDesc.Height = _windowHeight;                  // Match client height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // Pipeline destination
    scd.OutputWindow = hwnd;                                // Link to our Win32 window
    scd.SampleDesc.Count = 1;                               // No multi-sampling
    scd.Windowed = !_fullScreen;                            // Windowed mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // Create Device, Context, and Swap Chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &scd,
        &g_swapChain, &g_device, nullptr, &g_context
    );

    if (FAILED(hr)) return hr;

    ApplyChanges();

    for (Renderer* renderer : renderers)
    {
        hr = renderer->CreateResources(g_context.Get(), g_device.Get());
        if (FAILED(hr)) return hr;
    }

    ID3DBlob* VSBlob = nullptr;
    D3DReadFileToBlob(L"VertexShader.cso", &VSBlob);
    hr = g_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS);
    VSBlob->Release();
    if (FAILED(hr)) return hr;

    ID3DBlob* PSBlob = nullptr;
    D3DReadFileToBlob(L"PixelShader.cso", &PSBlob);
    hr = g_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS);
    PSBlob->Release();
    if (FAILED(hr)) return hr;


    return S_OK;
}

void FlowEngineDX11::DX11Protocol::Render()
{
    Clear({100,100,255,255});
    g_context->OMSetRenderTargets(
        1,
        g_renderTargetView.GetAddressOf(),
        nullptr);


    g_context->RSSetViewports(1, &viewport);

    g_context->PSSetShader(PS.Get(), nullptr, 0);
    g_context->VSSetShader(VS.Get(), nullptr, 0);

    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (Renderer* renderer : renderers)
    {
        renderer->Render(g_context.Get(), g_device.Get());
    }

    // Present back buffer to screen (1 = lock to VSync, 0 = uncapped)
    g_swapChain->Present(1, 0);
}

void FlowEngineDX11::DX11Protocol::ApplyChanges()
{
    g_context->OMSetRenderTargets(0, nullptr, nullptr);
    g_renderTargetView.Reset(); // Clear old references
	
    g_swapChain->ResizeBuffers(0, _windowWidth, _windowHeight, DXGI_FORMAT_UNKNOWN, 0);

    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    g_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &g_renderTargetView);
    g_context->OMSetRenderTargets(1, g_renderTargetView.GetAddressOf(), nullptr);

    viewport = { 0, 0, static_cast<float>(_windowWidth), static_cast<float>(_windowHeight), 0.0f, 1.0f };
    g_context->RSSetViewports(1, &viewport);
}

FlowEngineDX11::Renderer* FlowEngineDX11::DX11Protocol::AddRenderer(Renderer* renderer)
{
    renderers.push_back(renderer);
    return renderer;
}

void FlowEngineDX11::DX11Protocol::CleanUp()
{
    if (g_context) g_context->ClearState();
}

bool FlowEngineDX11::DX11Protocol::Init()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // 2. Register the DX11Protocol Class
    constexpr  wchar_t CLASS_NAME[] = L"DirectXEngineClass";

    WNDCLASSEX wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcStatic;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClassEx(&wc);

    DWORD style = WS_OVERLAPPEDWINDOW;

    // 3. Create the DX11Protocol
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, std::wstring(_windowTitle.begin(), _windowTitle.end()).c_str(),
        style,
        0, 0, _windowWidth, _windowHeight,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) {
        std::cout << "DX11Protocol Creation Failed!\n";
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);

    if (FAILED(InitD3D(hwnd))) {
        MessageBox(hwnd, L"Failed to initialize DirectX 11", L"Error", MB_OK);
        return false;
    }

    return true;
}

// Drawing functions

void FlowEngineDX11::DX11Protocol::Clear(const Color color)
{
    auto floats = color.ToFloatArray();
    const float colorArr[4] = {floats[0], floats[1], floats[2], floats[3]};

    g_context->ClearRenderTargetView(g_renderTargetView.Get(), colorArr);
}
