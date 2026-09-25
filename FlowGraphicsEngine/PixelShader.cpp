#include "PixelShader.h"
#include "DX11Protocol.h"

FlowEngineDX11::PixelShader::~PixelShader()
{
    if (PS) {
        PS->Release();
        PS = nullptr;
    }
}

HRESULT FlowEngineDX11::PixelShader::CreateShader(const LPCWSTR& psLocation, const DX11Protocol* dx11_protocol)
{
    auto g_device = dx11_protocol->GetDevice();
    ID3DBlob* PSBlob = nullptr;
    HRESULT hr = D3DReadFileToBlob(psLocation, &PSBlob);
    if (FAILED(hr)) return hr;
    hr = g_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS);
    PSBlob->Release();
    PSBlob = nullptr;
    if (FAILED(hr)) return hr;
    return S_OK;
}

ID3D11PixelShader* FlowEngineDX11::PixelShader::GetShader()
{
    return PS;
}

ID3D11PixelShader** FlowEngineDX11::PixelShader::GetShaderAddress()
{
    return &PS;
}
