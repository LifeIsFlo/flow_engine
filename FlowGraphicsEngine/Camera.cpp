#include "Camera.h"
#include "Transform.h"
#include <DirectXMathMatrix.inl>

#include "CameraBuffer.h"

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

	if (projectionMode == Orthographic)
	{
		auto projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, 0.0f, farZ);

		return viewMatrix * projectionMatrix;
	}
	if (projectionMode == Perspective)
	{
		auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, viewWidth / viewHeight, nearZ, farZ);
		
		return viewMatrix * projectionMatrix;
	}

	return DirectX::XMMatrixIdentity();
}

HRESULT FlowEngineDX11::Camera::CreateBuffers(ID3D11Device* g_device)
{
	D3D11_BUFFER_DESC transformBufferDesc{};
	transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformBufferDesc.ByteWidth = sizeof(CameraBuffer);
	transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = g_device->CreateBuffer(&transformBufferDesc, nullptr, &transformBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT FlowEngineDX11::Camera::UpdateBuffers(ID3D11DeviceContext* g_context)
{
	D3D11_MAPPED_SUBRESOURCE mappedCamera;
	HRESULT hr = g_context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTransform);

	if (SUCCEEDED(hr)) {
		auto transformMatrix = CreateGPUMatrix(camera);

		memcpy(mappedTransform.pData, &transformMatrix, sizeof(transformMatrix));

		g_context->Unmap(transformBuffer, 0);
	}
	else return hr;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Camera::GetRegistry()
{
	return ObjectRegistry::Camera;
}
