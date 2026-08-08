#include "Vector2.h"
#include "FWD.h"
#include "DX11Protocol.h"
#include "Renderer.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Shader.h"
#include <wrl/client.h>
#include "Input.h"
// Link required DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

FlowEngineDX11::DX11Protocol* FlowEngineDX11::DX11Protocol::current = nullptr;

FlowEngineDX11::DX11Protocol::DX11Protocol(const UINT& windowHeight, const UINT& windowWidth, const std::string& windowTitle, const bool& fullScreen)
{
    _renderHeight = windowHeight;
    _renderWidth = windowWidth;
    _windowTitle = windowTitle;
    _fullScreen = fullScreen;

    current = this;
}

bool FlowEngineDX11::DX11Protocol::Init()
{
    if (!mainCamera) return false;
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    constexpr  wchar_t CLASS_NAME[] = L"DirectXEngineClass";

    WNDCLASSEX wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcStatic;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClassEx(&wc);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;

    RECT rect = {0,0, _renderWidth, _renderHeight};
    AdjustWindowRect(&rect, style, FALSE);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, std::wstring(_windowTitle.begin(), _windowTitle.end()).c_str(),
        style,
        0, 0, rect.right - rect.left, rect.bottom - rect.top,
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

    if (FAILED(CreateResources()))
    {
	    MessageBox(hwnd, L"Couldn't make the proper resources", L"Error", MB_OK);
        return false;
    }

    Input::Init(hInstance, hwnd);

    return true;
}

HRESULT FlowEngineDX11::DX11Protocol::CreateResources()
{
    HRESULT hr;
    for (Renderer* renderer : renderers)
    {
        std::cout << "Creating resources for renderer";
        hr = renderer->CreateResources(g_context, g_device);
        if (FAILED(hr)) return hr;
        std::cout << "Resources for renderer created";
    }

    D3D11_INPUT_ELEMENT_DESC ILDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    shader = new Shader(L"PixelShader.cso", L"VertexShader.cso", ILDesc, 2);

    hr = shader->InitShader(g_device);
    if (FAILED(hr)) return hr;

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = g_device->CreateSamplerState(&samplerDesc, &texture2dSampler);
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT FlowEngineDX11::DX11Protocol::InitD3D(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;                                    // One back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Color format
    scd.BufferDesc.Width = _renderWidth;                    // Match client width
    scd.BufferDesc.Height = _renderHeight;                  // Match client height
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

    return S_OK;
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
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            RECT rect = { 0,0,  static_cast<LONG>(width), static_cast<LONG>(height) };

            GetClientRect(hwnd, &rect);

            _renderHeight = rect.bottom - rect.top;
            _renderWidth = rect.right - rect.left;

            ApplyChanges();
        }
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void FlowEngineDX11::DX11Protocol::Render()
{
    BeginFrame({100,100,255,255});

    for (Renderer* renderer : renderers)
    {
        renderer->UpdateAndRender(g_context, g_device, mainCamera);
    }

    EndFrame();
}

HRESULT FlowEngineDX11::DX11Protocol::ApplyChanges()
{
    g_context->OMSetRenderTargets(0, nullptr, nullptr);
    if (g_renderTargetView)
    {
        g_renderTargetView->Release();
        g_renderTargetView = nullptr;
    }

    if (g_backBuffer)
    {
        g_backBuffer->Release();
        g_backBuffer = nullptr;
    }
	
    HRESULT hr = g_swapChain->ResizeBuffers(0, _renderWidth, _renderHeight, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) return hr;

    hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&g_backBuffer));
    if (FAILED(hr)) return hr;

    hr = g_device->CreateRenderTargetView(g_backBuffer, nullptr, &g_renderTargetView);
    if (FAILED(hr)) return hr;

    g_context->OMSetRenderTargets(1, &g_renderTargetView, nullptr);

    mainCamera->viewHeight = _renderHeight;
    mainCamera->viewWidth = _renderWidth;
	mainCamera->CreateViewport();
    g_context->RSSetViewports(1, mainCamera->GetViewport());

    return S_OK;
}

FlowEngineDX11::Renderer* FlowEngineDX11::DX11Protocol::AddRenderer(Renderer* renderer)
{
    renderers.push_back(renderer);
    return renderer;
}

void FlowEngineDX11::DX11Protocol::CleanUp()
{
    if (g_context) g_context->ClearState();
    if (g_renderTargetView) {
        g_renderTargetView->Release();
        g_renderTargetView = nullptr;
    }
    if (g_backBuffer) {
        g_backBuffer->Release();
        g_backBuffer = nullptr;
    }
    if (g_device) {
        g_device->Release();
        g_device = nullptr;
    }
    if (g_context) {
        g_context->Release();
        g_context = nullptr;
    }
    if (g_swapChain) {
        g_swapChain->Release();
        g_swapChain = nullptr;
    }
    if (shader) {
        shader->Release();
        shader = nullptr;
    }
    if (texture2dSampler) {
        texture2dSampler->Release();
        texture2dSampler = nullptr;
    }
    Input::CleanUp();
}

// Drawing functions

void FlowEngineDX11::DX11Protocol::BeginFrame(const Color color)
{
    g_context->OMSetRenderTargets(
        1,
        &g_renderTargetView,
        nullptr);

    g_context->RSSetViewports(1, mainCamera->GetViewport());

    auto floats = color.ToFloatArray();
    const float colorArr[4] = {floats[0], floats[1], floats[2], floats[3]};

    g_context->ClearRenderTargetView(g_renderTargetView, colorArr);
    g_context->PSSetShader(shader->GetPixelShader(), nullptr, 0);
    g_context->VSSetShader(shader->GetVertexShader(), nullptr, 0);
    g_context->IASetInputLayout(shader->GetInputLayout());
    g_context->PSSetSamplers(0, 1, &texture2dSampler);
    g_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void FlowEngineDX11::DX11Protocol::EndFrame()
{
    g_swapChain->Present(1, 0);
}

