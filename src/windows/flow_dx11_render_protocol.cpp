#include "flow_dx11_render_protocol.h"
#include "flow_asset_database.h"
#include "flow_io.h"

namespace flow::win32
{

    DX11Protocol* DX11Protocol::current = nullptr;

    LRESULT CALLBACK DX11Protocol::WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return current->WindowProc(hwnd, uMsg, wParam, lParam);
    }

    LRESULT DX11Protocol::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_ACTIVATEAPP:
            if (g_swapChain)
            {
                if (wParam != FALSE)
                {
                    ApplyChanges();
                }
            }
            return 0;
        case WM_SIZE:
            // Handle window resizing safely if initialization is finished
            if (g_swapChain && wParam != SIZE_MINIMIZED)
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);

                RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};

                GetClientRect(hwnd, &rect);

                _renderHeight = rect.bottom - rect.top;
                _renderWidth = rect.right - rect.left;

                ApplyChanges();
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    HRESULT DX11Protocol::Init(HWND* hwnd, HINSTANCE* hInstance)
    {
        *hInstance = GetModuleHandle(nullptr);

        LPCSTR CLASS_NAME = "DirectXEngineClass";
        LPCSTR WINDOW_TITLE = _windowTitle.c_str();

        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProcStatic;
        wc.hInstance = *hInstance;
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        RegisterClassEx(&wc);

        DWORD style = WS_POPUP | WS_VISIBLE;

        RECT rect = {0, 0, static_cast<LONG>(_renderWidth), static_cast<LONG>(_renderHeight)};
        AdjustWindowRect(&rect, style, FALSE);

        *hwnd = CreateWindowEx(0, CLASS_NAME, WINDOW_TITLE, style, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                               nullptr, nullptr, *hInstance, nullptr);

        if (hwnd == nullptr)
        {
            std::cout << "DX11Protocol Creation Failed!\n";
            return E_FAIL;
        }

        ShowWindow(*hwnd, SW_SHOW);

        HRESULT hr = InitD3D(*hwnd);
        if (FAILED(hr))
            return hr;

        return S_OK;
    }

    HRESULT DX11Protocol::InitD3D(HWND hwnd)
    {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.Width = _renderWidth;
        scd.BufferDesc.Height = _renderHeight;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};

        // Create Device, Context, and Swap Chain
        HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                                   D3D11_CREATE_DEVICE_DEBUG, featureLevels, 1, D3D11_SDK_VERSION, &scd,
                                                   &g_swapChain, &g_device, nullptr, &g_context);
        if (FAILED(hr))
            return hr;

        ApplyChanges();

        return S_OK;
    }

    HRESULT DX11Protocol::CreateResources()
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> ILDesc = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"CAMERA_POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
             D3D11_INPUT_PER_INSTANCE_DATA, 1}};

        vertexShader = new DX11VertexShader(ILDesc);
        vertexShader->CreateShader(L"MainVS.cso", g_device);
        pixelShader = new DX11PixelShader();
        pixelShader->CreateShader(L"MainPS.cso", g_device);

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        HRESULT hr = g_device->CreateSamplerState(&samplerDesc, &texture2dSampler);
        if (FAILED(hr))
            return hr;

        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = true;                                   // 4. Turn blending ON/OFF
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;                     // 5. RGB Source multiplier
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;                // 6. RGB Destination multiplier
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                         // 7. RGB Equation operator
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;                      // 8. Alpha Source multiplier
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;                    // 9. Alpha Destination multiplier
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;                    // 10. Alpha Equation operator
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 11. Channel toggles
        blendDesc.AlphaToCoverageEnable = false;

        hr = g_device->CreateBlendState(&blendDesc, &g_blendState);
        if (FAILED(hr))
            return hr;

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = false;
        rasterizerDesc.MultisampleEnable = true;

        hr = g_device->CreateRasterizerState(&rasterizerDesc, &g_rasterizerState);
        if (FAILED(hr))
            return hr;

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        hr = g_device->CreateDepthStencilState(&depthStencilDesc, &g_depthStencilState);
        if (FAILED(hr))
            return hr;

        // Shadow pass

        return S_OK;
    }

    HRESULT DX11Protocol::ApplyChanges()
    {
        g_context->OMSetRenderTargets(0, nullptr, nullptr);

        if (g_depthStencilView)
        {
            g_depthStencilView->Release();
            g_depthStencilView = nullptr;
        }

        if (g_depthStencilBuffer)
        {
            g_depthStencilBuffer->Release();
            g_depthStencilBuffer = nullptr;
        }

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
        if (FAILED(hr))
            return hr;

        hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&g_backBuffer));
        if (FAILED(hr))
            return hr;

        hr = g_device->CreateRenderTargetView(g_backBuffer, nullptr, &g_renderTargetView);
        if (FAILED(hr))
            return hr;

        D3D11_TEXTURE2D_DESC depthStencilViewTextureDesc = {};
        depthStencilViewTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilViewTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilViewTextureDesc.SampleDesc.Count = 1;
        depthStencilViewTextureDesc.SampleDesc.Quality = 0;
        depthStencilViewTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewTextureDesc.Height = _renderHeight;
        depthStencilViewTextureDesc.Width = _renderWidth;
        depthStencilViewTextureDesc.MipLevels = 1;
        depthStencilViewTextureDesc.ArraySize = 1;
        depthStencilViewTextureDesc.CPUAccessFlags = 0;
        depthStencilViewTextureDesc.MiscFlags = 0;

        hr = g_device->CreateTexture2D(&depthStencilViewTextureDesc, nullptr, &g_depthStencilBuffer);
        if (FAILED(hr))
            return hr;

        hr = g_device->CreateDepthStencilView(g_depthStencilBuffer, nullptr, &g_depthStencilView);
        if (FAILED(hr))
            return hr;

        return S_OK;
    }

    void DX11Protocol::ClearState()
    {
        g_context->ClearState();
        g_context->Flush();
    }

    void DX11Protocol::CleanUp()
    {
        if (vertexShader)
        {
            delete vertexShader;
            vertexShader = nullptr;
        }
        if (pixelShader)
        {
            delete pixelShader;
            pixelShader = nullptr;
        }
        if (g_context)
        {
            g_context->ClearState();
            g_context->Flush();
        }
        if (g_blendState)
        {
            g_blendState->Release();
            g_blendState = nullptr;
        }
        if (texture2dSampler)
        {
            texture2dSampler->Release();
            texture2dSampler = nullptr;
        }
        if (g_renderTargetView)
        {
            g_renderTargetView->Release();
            g_renderTargetView = nullptr;
        }
        if (g_depthStencilBuffer)
        {
            g_depthStencilBuffer->Release();
            g_depthStencilBuffer = nullptr;
        }
        if (g_depthStencilState)
        {
            g_depthStencilState->Release();
            g_depthStencilState = nullptr;
        }
        if (g_depthStencilView)
        {
            g_depthStencilView->Release();
            g_depthStencilView = nullptr;
        }
        if (g_backBuffer)
        {
            g_backBuffer->Release();
            g_backBuffer = nullptr;
        }
        if (g_context)
        {
            g_context->Release();
            g_context = nullptr;
        }
        if (g_swapChain)
        {
            g_swapChain->Release();
            g_swapChain = nullptr;
        }
        if (g_rasterizerState)
        {
            g_rasterizerState->Release();
            g_rasterizerState = nullptr;
        }
        if (g_device)
        {
            ID3D11Debug* debug = nullptr;

            if (SUCCEEDED(g_device->QueryInterface(IID_PPV_ARGS(&debug))))
            {
                debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                debug->Release();
            }

            flow::io::Console::WriteLine("device references: " + std::to_string(g_device->Release()));
            g_device = nullptr;
        }
    }

    void DX11Protocol::PrepareShadowPass()
    {
        ID3D11ShaderResourceView* nullSRV = nullptr;
        g_context->PSSetShaderResources(0, 1, &nullSRV);
        g_context->VSSetShaderResources(0, 1, &nullSRV);
        g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        g_context->OMSetDepthStencilState(g_depthStencilState, 0);
        g_context->OMSetRenderTargets(0, nullptr, nullptr);
        g_context->PSSetShader(nullptr, nullptr, 0);
    }

    // Drawing functions
    void DX11Protocol::PrepareMainPass()
    {
        g_context->OMSetRenderTargets(1, &g_renderTargetView, g_depthStencilView);

        g_context->OMSetDepthStencilState(g_depthStencilState, 0);

        // g_context->RSSetViewports(1, activeCamera->GetViewport());
        g_context->RSSetState(g_rasterizerState);

        // auto floats = activeCamera->backgroundColor.ToFloatArray();
        const float colorArr[4] = {0.7f, 0.7f, 0.7f, 0.7f};

        g_context->ClearRenderTargetView(g_renderTargetView, colorArr);
        g_context->ClearDepthStencilView(g_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        g_context->PSSetSamplers(0, 1, &texture2dSampler);
        g_context->PSSetShader(pixelShader->GetShader(), nullptr, 0);
        g_context->VSSetShader(vertexShader->GetVertexShader(), nullptr, 0);
        g_context->IASetInputLayout(vertexShader->GetInputLayout());
        g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        constexpr float factor[4] = {1, 1, 1, 1};
        g_context->OMSetBlendState(g_blendState, factor, 0xffffffff);
    }

    void DX11Protocol::EndFrame()
    {
        g_swapChain->Present(_vSync, 0);
    }

    // Getters
    ID3D11DeviceContext* DX11Protocol::GetContext() const
    {
        return g_context;
    }

    ID3D11Device* DX11Protocol::GetDevice() const
    {
        return g_device;
    }

    IDXGISwapChain* DX11Protocol::GetSwapChain() const
    {
        return g_swapChain;
    }
} // namespace flow::win32