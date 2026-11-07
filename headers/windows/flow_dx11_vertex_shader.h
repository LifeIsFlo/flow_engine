#pragma once
#include "flow_dxfwd.h"
#include <vector>

namespace flow::win32
{
    class DX11VertexShader
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_ILDesc;
        ID3D11VertexShader* VS = nullptr;
        ID3D11InputLayout* IL = nullptr;

    public:
        ~DX11VertexShader();
        DX11VertexShader(const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc);
        HRESULT CreateShader(const LPCWSTR& vsLocation, ID3D11Device* g_device);
        ID3D11VertexShader* GetVertexShader();
        ID3D11InputLayout* GetInputLayout();
    };
} // namespace flow::win32
