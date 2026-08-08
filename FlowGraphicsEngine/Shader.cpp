#include "Shader.h"

#include <d3dcompiler.h>
#include <Windows.h>

FlowEngineDX11::Shader::Shader(LPCWSTR psLocation, LPCWSTR vsLocation, D3D11_INPUT_ELEMENT_DESC* ilDesc, UINT numElements)
{
	m_psLocation = psLocation;
	m_vsLocation = vsLocation;
	m_numElements = numElements;
	m_ILDesc = ilDesc;
}

void FlowEngineDX11::Shader::Release()
{
	if (VS) {
		VS->Release();
		VS = nullptr;
	}
	if (PS) {
		PS->Release();
		PS = nullptr;
	}
	if (IL) {
		IL->Release();
		IL = nullptr;
	}
}

HRESULT FlowEngineDX11::Shader::InitShader(ID3D11Device* g_device)
{
	ID3DBlob* VSBlob = nullptr;
	D3DReadFileToBlob(m_vsLocation, &VSBlob);
	HRESULT hr = g_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS);
	if (FAILED(hr)) return hr;

	ID3DBlob* PSBlob = nullptr;
	D3DReadFileToBlob(m_psLocation, &PSBlob);
	hr = g_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &PS);
	PSBlob->Release();
	PSBlob = nullptr;
	if (FAILED(hr)) return hr;

	hr = g_device->CreateInputLayout(m_ILDesc, m_numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &IL);
	VSBlob->Release();
	VSBlob = nullptr;
	if (FAILED(hr)) return hr;

	return S_OK;
}

ID3D11VertexShader* FlowEngineDX11::Shader::GetVertexShader()
{
	return VS;
}

ID3D11PixelShader* FlowEngineDX11::Shader::GetPixelShader()
{
	return PS;
}

ID3D11InputLayout* FlowEngineDX11::Shader::GetInputLayout()
{
	return IL;
}
