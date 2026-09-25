#pragma once
#include <d3d11.h>

#include "FWD.h"
#include "Object.h"

namespace FlowEngineDX11
{
	class VertexShader : public Object
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ILDesc;
		ID3D11VertexShader* VS = nullptr;
		ID3D11InputLayout* IL = nullptr;
	public:
		~VertexShader();
		VertexShader(const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc);
		HRESULT CreateShader(const LPCWSTR& vsLocation, const DX11Protocol* dx11_protocol);
		ID3D11VertexShader* GetVertexShader();
		ID3D11InputLayout* GetInputLayout();
	};
}
