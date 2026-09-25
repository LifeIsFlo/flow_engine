#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Component.h"
#include "Color.h"

namespace FlowEngineDX11
{
	class Camera : public Component
	{
	public:
		enum ProjectionMode : uint8_t { Perspective, Orthographic };
		float fov = 45;
		float offsetX = 0;
		float offsetY = 0;
		float viewHeight = 600;
		float viewWidth = 800;
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		Color backgroundColor{};
		ProjectionMode projectionMode = Perspective;
	private:
		ID3D11Buffer* cameraBuffer = nullptr;
		D3D11_VIEWPORT m_viewport = {};
	public:
		~Camera() override;
		DirectX::XMMATRIX CreateViewMatrix();
		DirectX::XMMATRIX CreateProjectionMatrix();
		HRESULT CreateBuffers(const DX11Protocol* dx11_protocol);
		HRESULT UpdateBuffers(const DX11Protocol* dx11_protocol);

		void ApplyChanges(DX11Protocol* dx11_protocol);

		D3D11_VIEWPORT* GetViewport();

		ObjectRegistry GetRegistry() override;
	};
}
