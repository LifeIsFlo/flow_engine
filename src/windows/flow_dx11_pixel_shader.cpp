#include "flow_dx11_pixel_shader.h"
#include "flow_dxfwd.h"

namespace flow::win32
{
    DX11PixelShader::~DX11PixelShader()
    {
        if (PS)
        {
            PS->Release();
            PS = nullptr;
        }
    }

    HRESULT DX11PixelShader::CreateShader(const LPCWSTR& psLocation, ID3D11Device* g_device)
    {
        ID3DBlob* PSBlob = nullptr;
        HRESULT hr = D3DReadFileToBlob(psLocation, &PSBlob);
        if (FAILED(hr))
            return hr;
        hr = g_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS);
        PSBlob->Release();
        PSBlob = nullptr;
        if (FAILED(hr))
            return hr;
        return S_OK;
    }

    ID3D11PixelShader* DX11PixelShader::GetShader()
    {
        return PS;
    }

    ID3D11PixelShader** DX11PixelShader::GetShaderAddress()
    {
        return &PS;
    }
}