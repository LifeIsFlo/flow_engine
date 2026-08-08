#include "Camera.h"
#include "Transform.h"
#include <DirectXMathMatrix.inl>

HRESULT FlowEngineDX11::Camera::CreateViewport()
{
	m_viewport = { offsetX, offsetY, viewWidth, viewHeight, 0, 1.0 };
	return S_OK;
}

D3D11_VIEWPORT* FlowEngineDX11::Camera::GetViewport()
{
	return &m_viewport;
}

DirectX::XMMATRIX FlowEngineDX11::Camera::CreateViewProjectionMatrix()
{
	auto transformMatrix = transform->CreateMatrix();

	DirectX::XMVECTOR det;
	auto viewMatrix = DirectX::XMMatrixInverse(&det, transformMatrix);

	if (projectionMode == ProjectionMode::Orthographic)
	{
		auto projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, 0.0f, farZ);

		return viewMatrix * projectionMatrix;
	}
	if (projectionMode == ProjectionMode::Perspective)
	{
		auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, viewWidth / viewHeight, nearZ, farZ);
		
		return viewMatrix * projectionMatrix;
	}

	return DirectX::XMMatrixIdentity();
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Camera::GetRegistry()
{
	return ObjectRegistry::Camera;
}
