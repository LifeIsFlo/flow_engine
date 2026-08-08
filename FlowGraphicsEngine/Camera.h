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
		float fov = 60;
		float offsetX = 0;
		float offsetY = 0;
		float viewHeight = 600;
		float viewWidth = 800;
		float nearZ = 0.0f;
		float farZ = 1.0f;
		ProjectionMode projectionMode = Orthographic;
	private:
		D3D11_VIEWPORT m_viewport = {};
	public:
		HRESULT CreateViewport();
		D3D11_VIEWPORT* GetViewport();
		DirectX::XMMATRIX CreateViewProjectionMatrix();
		ObjectRegistry GetRegistry() override;
	};
}
