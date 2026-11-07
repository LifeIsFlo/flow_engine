#pragma once
#include "flow_dxfwd.h"

namespace flow::win32
{
    class DX11PixelShader
    {
        ID3D11PixelShader* PS = nullptr;

    public:
        ~DX11PixelShader();
        HRESULT CreateShader(const LPCWSTR& psLocation, ID3D11Device* g_device);

        ID3D11PixelShader* GetShader();
        ID3D11PixelShader** GetShaderAddress();
    };
} // namespace flow::win32
