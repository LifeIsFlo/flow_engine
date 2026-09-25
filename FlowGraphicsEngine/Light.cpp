#include "Light.h"
#include "Transform.h"
#include "LightingProtocol.h"
#include "ViewProjectionBuffer.h"
#include "Renderer.h"

FlowEngineDX11::Light::~Light()
{
    if (viewProjectionBuffer) {
        viewProjectionBuffer->Release();
        viewProjectionBuffer = nullptr;
    }
}

void FlowEngineDX11::Light::ShadowPass(const DX11Protocol* dx11_protocol, const LightingProtocol* lighting_protocol)
{
    auto g_device = dx11_protocol->GetDevice();
    auto g_context = dx11_protocol->GetContext();
	auto shadows = lighting_protocol->GetShadows();

    const DirectX::XMVECTOR directions[6] =
    {
        DirectX::XMVectorSet(1,  0,  0, 0), // +X
        DirectX::XMVectorSet(-1,  0,  0, 0), // -X

        DirectX::XMVectorSet(0,  1,  0, 0), // +Y
        DirectX::XMVectorSet(0, -1,  0, 0), // -Y

        DirectX::XMVectorSet(0,  0,  1, 0), // +Z
        DirectX::XMVectorSet(0,  0, -1, 0), // -Z
    };

    const DirectX::XMVECTOR ups[6] =
    {
        DirectX::XMVectorSet(0, 1, 0, 0), // +X
        DirectX::XMVectorSet(0, 1, 0, 0), // -X

        DirectX::XMVectorSet(0, 0, -1, 0), // +Y
        DirectX::XMVectorSet(0, 0,  1, 0), // -Y

        DirectX::XMVectorSet(0, 1, 0, 0), // +Z
        DirectX::XMVectorSet(0, 1, 0, 0), // -Z
    };

    DirectX::XMMATRIX viewMatrices[6];

    auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(90, 1.0f, 0.01f, 50.0f);

    DirectX::XMFLOAT4X4 projectionMatrixGPU;
    DirectX::XMStoreFloat4x4(
        &projectionMatrixGPU,
        DirectX::XMMatrixTranspose(projectionMatrix)
    );

	for (UINT i = 0; i < 6U; i++) 
	{
        viewMatrices[i] =
            DirectX::XMMatrixLookToLH(
                DirectX::XMVectorSet(transform->position.x, transform->position.y, transform->position.z, 1.0f),
                directions[i],
                ups[i]
            );

        D3D11_MAPPED_SUBRESOURCE mappedViewProjection{};
        HRESULT hr = g_context->Map(viewProjectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedViewProjection);

        if (SUCCEEDED(hr)) 
        {
            DirectX::XMFLOAT4X4 viewMatrixGPU;
            DirectX::XMStoreFloat4x4(
                &viewMatrixGPU,
                DirectX::XMMatrixTranspose(viewMatrices[i])
            );

            ViewProjectionBuffer lightMatrix = { viewMatrixGPU, projectionMatrixGPU, transform->position, 0.0f };

            memcpy(mappedViewProjection.pData, &lightMatrix, sizeof(lightMatrix));

            g_context->Unmap(viewProjectionBuffer, 0);

            g_context->ClearDepthStencilView(shadowMap->GetDSV(i), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            g_context->VSSetConstantBuffers(1, 1, &viewProjectionBuffer);
            g_context->OMSetRenderTargets(0, nullptr, shadowMap->GetDSV(i));
            g_context->RSSetViewports(1, shadowMap->GetViewport());

            for (auto shadow : shadows)
            {
                shadow->Render(dx11_protocol);
            }
        }
	}
}

HRESULT FlowEngineDX11::Light::CreateResources(const DX11Protocol* dx11_protocol)
{
    D3D11_BUFFER_DESC viewProjectionBufferDesc{};
    viewProjectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    viewProjectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    viewProjectionBufferDesc.ByteWidth = sizeof(ViewProjectionBuffer);
    viewProjectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = dx11_protocol->GetDevice()->CreateBuffer(&viewProjectionBufferDesc, nullptr, &viewProjectionBuffer);
    if (FAILED(hr)) return hr;

    ShadowMap* rawShadowMap = nullptr;

    hr = ShadowMap::Create(dx11_protocol, 2048 * 2, &rawShadowMap);
    if (FAILED(hr)) return hr;

    shadowMap.reset(rawShadowMap);

    return S_OK;
}

FlowEngineDX11::LightData FlowEngineDX11::Light::GetLightData()
{
	return { transform->position, radius, GPUColor(color), intensity, falloffEnabled };
}

FlowEngineDX11::ShadowMap* FlowEngineDX11::Light::GetShadowMap()
{
    return shadowMap.get();
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Light::GetRegistry()
{
    return ObjectRegistry::Light;
}
