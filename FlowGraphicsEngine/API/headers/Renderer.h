#pragma once
#include <DirectXMath.h>

#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "DX11Protocol.h"
#include <random>
#include <span>

namespace FlowEngineDX11 
{
	class Renderer : public Component
	{
	protected:
		ID3D11Buffer* transformBuffer = nullptr;
	public:
		bool castShadows = true;
		~Renderer() override;
		HRESULT CreateBuffers(const DX11Protocol* dx11_protocol);
		void UpdateBuffers(const DX11Protocol* dx11_protocol);
		virtual void PrepareMainPass(const DX11Protocol* dx11_protocol) = 0;
		virtual void Render(const DX11Protocol* dx11_protocol) = 0;
	protected:
		HRESULT UpdateTransformBuffer(const DX11Protocol* dx11_protocol);
	public:
		ObjectRegistry GetRegistry() override;
	};
}
