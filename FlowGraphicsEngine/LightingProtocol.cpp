#include "LightingProtocol.h"
#include "Light.h"
#include "LightData.h"
#include "VertexShader.h"
#include "PixelShader.h" 
#include "DX11Protocol.h"
#include "Color.h"
#include "FWD.h"
#include "FlowEngine.h"
#include "AssetDatabase.h"

namespace FlowEngineDX11 
{
    void LightingProtocol::ShadowPass(FlowEngine* engine, const DX11Protocol* dx11_protocol)
    {
        auto g_context = dx11_protocol->GetContext();

        g_context->RSSetState(g_shadowRasterizerState);
        g_context->OMSetBlendState(g_shadowBlendState, nullptr, 0xffffffff);
        g_context->VSSetShader(ShadowVS->GetVertexShader(), nullptr, 0);
        g_context->PSSetShader(ShadowPS->GetShader(), nullptr, 0);
        g_context->IASetInputLayout(ShadowVS->GetInputLayout());

        for (Light* light : lights) {
            light->ShadowPass(dx11_protocol, this);
            //light->GetShadowMap()->Save(dx11_protocol);
        }

        //engine->RequestExit();
    }


    void LightingProtocol::MainPass(const DX11Protocol* dx11_protocol)
    {
        if(g_lightDataSRV) dx11_protocol->GetContext()->PSSetShaderResources(1, 1, &g_lightDataSRV);
        if (g_shadowDataSRV) dx11_protocol->GetContext()->PSSetShaderResources(2, 1, &g_shadowDataSRV);
    }

    HRESULT LightingProtocol::CreateResources(const DX11Protocol* dx11_protocol)
    {
        auto g_device = dx11_protocol->GetDevice();

        // Shaders
        std::vector<D3D11_INPUT_ELEMENT_DESC> ILDesc =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"CAMERA_POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
        };

        ShadowVS = AssetDatabase::LoadVertexShader("ShadowVS.cso", ILDesc);
        ShadowPS = AssetDatabase::LoadPixelShader("ShadowPS.cso");

        // Light buffer
        std::vector<LightData> g_lightData{};
        g_lightData.reserve(lights.size());

        for (auto light : lights)
        {
            g_lightData.push_back(light->GetLightData());
        }

        if (g_lightData.empty()) 
        {
            g_lightBuffer = nullptr;
            g_lightDataSRV = nullptr;
            return S_OK;
        }

        D3D11_BUFFER_DESC lightBufferDesc{};
        lightBufferDesc.ByteWidth = sizeof(LightData) * g_lightData.size();
        lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        lightBufferDesc.CPUAccessFlags = 0;
        lightBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        lightBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        lightBufferDesc.StructureByteStride = sizeof(LightData);

        D3D11_SUBRESOURCE_DATA lightBufferInitData{};
        lightBufferInitData.pSysMem = g_lightData.data();

        HRESULT hr = g_device->CreateBuffer(&lightBufferDesc, &lightBufferInitData, &g_lightBuffer);
        if (FAILED(hr)) return hr;

        D3D11_SHADER_RESOURCE_VIEW_DESC lightBufferSRVdesc{};
        lightBufferSRVdesc.Format = DXGI_FORMAT_UNKNOWN;
        lightBufferSRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        lightBufferSRVdesc.Buffer.FirstElement = 0;
        lightBufferSRVdesc.Buffer.NumElements = g_lightData.size();

        hr = g_device->CreateShaderResourceView(g_lightBuffer, &lightBufferSRVdesc, &g_lightDataSRV);
        if (FAILED(hr)) return hr;

        // Shadows
        D3D11_RASTERIZER_DESC shadowRasterizerDesc = {};
        shadowRasterizerDesc.FillMode = D3D11_FILL_SOLID;
        shadowRasterizerDesc.CullMode = D3D11_CULL_BACK;
        shadowRasterizerDesc.FrontCounterClockwise = false;
        shadowRasterizerDesc.DepthBias = 1000;
        shadowRasterizerDesc.SlopeScaledDepthBias = 1.0f;
        shadowRasterizerDesc.DepthBiasClamp = 0;
        shadowRasterizerDesc.DepthClipEnable = true;

        hr = g_device->CreateRasterizerState(&shadowRasterizerDesc, &g_shadowRasterizerState);
        if (FAILED(hr)) return hr;

        D3D11_BLEND_DESC shadowBlendDesc = {};
        shadowBlendDesc.RenderTarget[0].BlendEnable = false;
        shadowBlendDesc.AlphaToCoverageEnable = false;

        hr = g_device->CreateBlendState(&shadowBlendDesc, &g_shadowBlendState);
        if (FAILED(hr)) return hr;

        for (Light* light : lights) {
            hr = light->CreateResources(dx11_protocol);
            if (FAILED(hr)) return hr;
        }

        D3D11_TEXTURE2D_DESC shadowCubemapsDesc = {};
        shadowCubemapsDesc.Width = 4096;
        shadowCubemapsDesc.Height = 4096;
        shadowCubemapsDesc.MipLevels = 1;
        shadowCubemapsDesc.ArraySize = 6 * lights.size();
        shadowCubemapsDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        shadowCubemapsDesc.SampleDesc.Count = 1;
        shadowCubemapsDesc.SampleDesc.Quality = 0;
        shadowCubemapsDesc.Usage = D3D11_USAGE_DEFAULT;
        shadowCubemapsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        shadowCubemapsDesc.CPUAccessFlags = 0;
        shadowCubemapsDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        hr = g_device->CreateTexture2D(&shadowCubemapsDesc, nullptr, &g_shadowDataCubeMaps);
        if (FAILED(hr)) return hr;

        D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc{};
        shadowSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
        shadowSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
        shadowSRVDesc.TextureCubeArray.MipLevels = 1;
        shadowSRVDesc.TextureCubeArray.NumCubes = lights.size();
        shadowSRVDesc.TextureCubeArray.First2DArrayFace = 0;
        shadowSRVDesc.TextureCubeArray.MostDetailedMip = 0;

        hr = g_device->CreateShaderResourceView(g_shadowDataCubeMaps, &shadowSRVDesc, &g_shadowDataSRV);
        if (FAILED(hr)) return hr;
    }

    Light* LightingProtocol::AddLight(Light* light)
    {
        lights.push_back(light);
        return light;
    }
    void LightingProtocol::RemoveLight(Light* light)
    {
        std::erase_if(lights, [light](const Light* element)
        {
            return light == element;
        });
    }
    Renderer* LightingProtocol::AddShadow(Renderer* renderer)
    {
        shadows.push_back(renderer);
        return renderer;
    }
    void LightingProtocol::RemoveShadow(Renderer* renderer)
    {
        std::erase_if(shadows, [renderer](const Renderer* element) 
        {
            return renderer == element;
        });
    }

    std::vector<Renderer*> LightingProtocol::GetShadows() const
    {
        return shadows;
    }

    void LightingProtocol::ClearState()
    {
        if (g_lightBuffer) {
            g_lightBuffer->Release();
            g_lightBuffer = nullptr;
        }

        if (g_lightDataSRV) {
            g_lightDataSRV->Release();
            g_lightDataSRV = nullptr;
        }

        if (g_shadowDataSRV) {
            g_shadowDataSRV->Release();
            g_shadowDataSRV = nullptr;
        }

        if (g_shadowBlendState) {
            g_shadowBlendState->Release();
            g_shadowBlendState = nullptr;
        }

        if (g_shadowRasterizerState) {
            g_shadowRasterizerState->Release();
            g_shadowRasterizerState = nullptr;
        }

        if (g_shadowDataCubeMaps) {
            g_shadowDataCubeMaps->Release();
            g_shadowDataCubeMaps = nullptr;
        }

        lights.clear();
        lights.shrink_to_fit();
        shadows.clear();
        shadows.shrink_to_fit();
    }

    LightingProtocol::~LightingProtocol()
    {
        ClearState();
    }
}