#include "flow_dx11_vertex_shader.h"

namespace flow::win32
{
    DX11VertexShader::~DX11VertexShader()
    {
        if (VS)
        {
            VS->Release();
            VS = nullptr;
        }
        if (IL)
        {
            IL->Release();
            IL = nullptr;
        }
    }

    DX11VertexShader::DX11VertexShader(const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc)
    {
        m_ILDesc = ILDesc;
    }

    HRESULT DX11VertexShader::CreateShader(const LPCWSTR& vsLocation, ID3D11Device* g_device)
    {
        ID3DBlob* VSBlob = nullptr;
        HRESULT hr = D3DReadFileToBlob(vsLocation, &VSBlob);
        if (FAILED(hr))
            return hr;

        hr = g_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS);
        if (FAILED(hr))
        {
            VSBlob->Release();
            VSBlob = nullptr;
            return hr;
        }

        hr = g_device->CreateInputLayout(m_ILDesc.data(), m_ILDesc.size(), VSBlob->GetBufferPointer(),
                                         VSBlob->GetBufferSize(), &IL);
        VSBlob->Release();
        VSBlob = nullptr;
        if (FAILED(hr))
            return hr;

        return S_OK;
    }

    ID3D11VertexShader* DX11VertexShader::GetVertexShader()
    {
        return VS;
    }

    ID3D11InputLayout* DX11VertexShader::GetInputLayout()
    {
        return IL;
    }
} // namespace flow::win32