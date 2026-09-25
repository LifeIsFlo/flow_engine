#pragma once
#include "FWD.h"
#include "Color.h"
#include "Object.h"

namespace FlowEngineDX11
{
	class Material :
		public Object
	{
		std::string shaderLocation;
		std::string textureLocation;
		Color baseColor;
		ID3D11PixelShader* shader;
		ID3D11SamplerState* textureSampler;

	public:
		HRESULT CreateBuffers(const DX11Protocol* dx11_protocol);

	};
}
