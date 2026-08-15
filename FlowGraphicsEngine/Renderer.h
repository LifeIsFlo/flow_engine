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
		ID3D11Buffer* indexBuffer = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* transformBuffer = nullptr;
	public:
		~Renderer() override;
		virtual HRESULT CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device) = 0;
		HRESULT CreateBuffers(ID3D11Device* g_device, std::span<const unsigned int> indices, std::span<const Vertex> vertices);
		void UpdateAndRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera);
	protected:
		virtual void Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera) = 0;
	private:
		DirectX::XMFLOAT4X4 CreateGPUMatrix(Camera* camera);
		HRESULT UpdateTransformBuffer(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera);
	public:
		ObjectRegistry GetRegistry() override;
	};
}
