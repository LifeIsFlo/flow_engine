#pragma once
#include <DirectXMath.h>

#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "DX11Protocol.h"
#include <random>
#include <span>

namespace FlowEngineDX11 {
	class Renderer : public Component
	{
	protected:
		ID3D11Buffer* transformBuffer = nullptr;
	public:
		~Renderer() override;
		virtual HRESULT CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device) = 0;
		HRESULT CreateBuffers(ID3D11Device* g_device, ID3D11DeviceContext* g_context);
		void UpdateAndRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device);
	protected:
		virtual void Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device) = 0;
	private:
		HRESULT UpdateTransformBuffer(ID3D11DeviceContext* g_context, ID3D11Device* g_device);
	public:
		ObjectRegistry GetRegistry() override;
	};
}
