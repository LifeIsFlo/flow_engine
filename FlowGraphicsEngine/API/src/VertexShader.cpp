#include "FWD.h"
#include "VertexShader.h"
#include "DX11Protocol.h"

namespace FlowEngineDX11 {

	FlowEngineDX11::VertexShader::~VertexShader()
	{
		if (VS) {
			VS->Release();
			VS = nullptr;
		}
		if (IL) {
			IL->Release();
			IL = nullptr;
		}
	}

	VertexShader::VertexShader(const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc)
	{
		m_ILDesc = ILDesc;
	}

	HRESULT VertexShader::CreateShader(const LPCWSTR& vsLocation, const DX11Protocol* dx11_protocol)
	{
		auto g_device = dx11_protocol->GetDevice();
		ID3DBlob* VSBlob = nullptr;
		HRESULT hr = D3DReadFileToBlob(vsLocation, &VSBlob);
		if (FAILED(hr)) return hr;

		hr = g_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &VS);
		if (FAILED(hr)) {
			VSBlob->Release();
			VSBlob = nullptr;
			return hr;
		}

		auto bp = VSBlob->GetBufferPointer();
		auto bs = VSBlob->GetBufferSize();

		hr = g_device->CreateInputLayout(m_ILDesc.data(), m_ILDesc.size(), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &IL);
		VSBlob->Release();
		VSBlob = nullptr;
		if (FAILED(hr)) return hr;

		return S_OK;
	}

	ID3D11VertexShader* VertexShader::GetVertexShader()
	{
		return VS;
	}

	ID3D11InputLayout* VertexShader::GetInputLayout()
	{
		return IL;
	}
}