#pragma once
#include <d3d11.h>

#include "FWD.h"

namespace FlowEngineDX11
{
	class Shader
	{
		ID3D11VertexShader* VS = nullptr;
		ID3D11PixelShader* PS = nullptr;
		ID3D11InputLayout* IL = nullptr;
		D3D11_INPUT_ELEMENT_DESC* m_ILDesc;
		LPCWSTR m_psLocation, m_vsLocation;
		UINT m_numElements;
	public:
		Shader(LPCWSTR psLocation, LPCWSTR vsLocation, D3D11_INPUT_ELEMENT_DESC* ilDesc, UINT numElements);
		void Release();
		HRESULT InitShader(ID3D11Device* g_device);
		ID3D11VertexShader* GetVertexShader();
		ID3D11PixelShader* GetPixelShader();
		ID3D11InputLayout* GetInputLayout();
	};
}
