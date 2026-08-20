#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Component.h"

namespace FlowEngineDX11
{
	class Camera :
		public Component
	{
	public:
		enum ProjectionMode: uint8_t { Perspective, Orthographic };
		float fov = 45;
		float offsetX = 0;
		float offsetY = 0;
		float viewHeight = 600;
		float viewWidth = 800;
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		ProjectionMode projectionMode = Orthographic;
	private:
		D3D11_VIEWPORT m_viewport = {};
	public:
		HRESULT CreateViewport();
		D3D11_VIEWPORT* GetViewport();
		DirectX::XMMATRIX CreateViewProjectionMatrix();
		HRESULT CreateBuffers(ID3D11Device* g_device);
		HRESULT UpdateBuffers(ID3D11DeviceContext* g_context);
		ObjectRegistry GetRegistry() override;
	};
}
