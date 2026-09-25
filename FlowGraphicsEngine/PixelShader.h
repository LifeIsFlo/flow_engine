#pragma once
#include "FWD.h"
#include "Object.h"

namespace FlowEngineDX11 
{
    class PixelShader :
        public Object
    {
        ID3D11PixelShader* PS = nullptr;
    public:
        ~PixelShader();
        HRESULT CreateShader(const LPCWSTR& psLocation, const DX11Protocol* dx11_protocol);

        ID3D11PixelShader* GetShader();
        ID3D11PixelShader** GetShaderAddress();
    };
}
