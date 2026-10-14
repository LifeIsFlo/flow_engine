#include "ShadowMap.h"
#include "DX11Protocol.h"
#include "Color.h"
#include <filesystem>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

FlowEngineDX11::ShadowMap::~ShadowMap()
{
    if (cubeMap) {
        cubeMap->Release();
        cubeMap = nullptr;
    }

    if (stagedCubeMap) {
        stagedCubeMap->Release();
        stagedCubeMap = nullptr;
    }

    for (auto dsv : depthViews) {
        if (dsv) {
            dsv->Release();
        }
    }
    if (depthSRV) {
        depthSRV->Release();
        depthSRV = nullptr;
    }
}

HRESULT FlowEngineDX11::ShadowMap::Create(const DX11Protocol* dx11_protocol, const UINT& resolution, ShadowMap** shadowMap)
{
    auto g_device = dx11_protocol->GetDevice();

    (*shadowMap) = new ShadowMap(resolution);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = resolution;
    desc.Height = resolution;
    desc.MipLevels = 1;
    desc.ArraySize = 6;
    desc.Format = DXGI_FORMAT_R32_TYPELESS;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    HRESULT hr = g_device->CreateTexture2D(&desc, nullptr, (*shadowMap)->GetCubeMapAddress());
    if (FAILED(hr)) return hr;

    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width = resolution;
    stagingDesc.Height = resolution;
    stagingDesc.MipLevels = 1;
    stagingDesc.ArraySize = 6;
    stagingDesc.Format = DXGI_FORMAT_R32_FLOAT;
    stagingDesc.SampleDesc.Count = 1;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.MiscFlags = 0;

    hr = g_device->CreateTexture2D(&stagingDesc, nullptr, &(*shadowMap)->stagedCubeMap);
    if (FAILED(hr)) return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    depthStencilViewDesc.Texture2DArray.ArraySize = 1;
    depthStencilViewDesc.Texture2DArray.MipSlice = 0;

    for (UINT i = 0; i < 6; i++) {
        depthStencilViewDesc.Texture2DArray.FirstArraySlice = i;

        hr = g_device->CreateDepthStencilView(
            (*shadowMap)->GetCubeMap(),
            &depthStencilViewDesc,
            (*shadowMap)->GetDSVAddress(i)
        );
        if (FAILED(hr)) return hr;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = 1;
    srvDesc.TextureCube.MostDetailedMip = 0;

    hr = g_device->CreateShaderResourceView((*shadowMap)->GetCubeMap(), &srvDesc, (*shadowMap)->GetDepthSRVAddress());
    if (FAILED(hr)) return hr;

    (*shadowMap)->m_viewport = {
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width = static_cast<float>(resolution),
        .Height = static_cast<float>(resolution),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    return S_OK;
}

void FlowEngineDX11::ShadowMap::Save(const DX11Protocol* dx11_protocol)
{
    auto g_context = dx11_protocol->GetContext();

    g_context->CopyResource(stagedCubeMap, cubeMap);

    D3D11_MAPPED_SUBRESOURCE mappedShadowMap{};
    HRESULT hr = g_context->Map(stagedCubeMap, 0, D3D11_MAP_READ, 0, &mappedShadowMap);

    if (SUCCEEDED(hr))
    {
        std::vector<Color> colorData(m_resolution * m_resolution);
        std::vector<float> depthData(m_resolution * m_resolution);

        const float* data = static_cast<const float*>(mappedShadowMap.pData);

        for (UINT y = 0; y < m_resolution; ++y)
        {
            const float* row = reinterpret_cast<const float*>(
                static_cast<const uint8_t*>(mappedShadowMap.pData) +
                y * mappedShadowMap.RowPitch
                );

            std::copy(
                row,
                row + m_resolution,
                depthData.begin() + y * m_resolution
            );
        }

        for (size_t i = 0; i < depthData.size(); ++i)
        {
            float raw = depthData[i];

            uint8_t value = static_cast<uint8_t>(( 0.01f / (1.01f - raw)) * 255.0f);

            colorData[i] = Color(
                value,
                value,
                value,
                255
            );
        }

        float minDepth = FLT_MAX;
        float maxDepth = -FLT_MAX;

        for (float depth : depthData)
        {
            minDepth = min(minDepth, depth);
            maxDepth = max(maxDepth, depth);
        }

        flowio::Console::WriteLine("Depth range: " + std::to_string(minDepth) + " - " + std::to_string(maxDepth));

        auto path = std::filesystem::current_path() / "shadowMap.png";

        flowio::Console::WriteLine(path.string().c_str());

        if (!stbi_write_png(path.string().c_str(), m_resolution, m_resolution, 4, colorData.data(), m_resolution * 4)) {
            flowio::Console::WriteLine("couldn't make shadowMap.png");
        }
        g_context->Unmap(stagedCubeMap, 0);
    }
}

UINT FlowEngineDX11::ShadowMap::GetResolution() const
{
    return m_resolution;
}

D3D11_VIEWPORT* FlowEngineDX11::ShadowMap::GetViewport()
{
    return &m_viewport;
}

ID3D11Texture2D* FlowEngineDX11::ShadowMap::GetCubeMap() const
{
    return cubeMap;
}

ID3D11Texture2D** FlowEngineDX11::ShadowMap::GetCubeMapAddress()
{
    return &cubeMap;
}

ID3D11DepthStencilView* FlowEngineDX11::ShadowMap::GetDSV(const int& index) const
{
    return depthViews[index];
}

ID3D11DepthStencilView** FlowEngineDX11::ShadowMap::GetDSVAddress(const int& index)
{
    return &depthViews[index];
}

ID3D11ShaderResourceView* FlowEngineDX11::ShadowMap::GetDepthSRV() const
{
    return depthSRV;
}

ID3D11ShaderResourceView** FlowEngineDX11::ShadowMap::GetDepthSRVAddress()
{
    return &depthSRV;
}
