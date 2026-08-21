#include "Camera.h"
#include "Transform.h"
#include <DirectXMathMatrix.inl>

#include "ViewProjectionBuffer.h"

FlowEngineDX11::Camera::~Camera()
{
	if (cameraBuffer) {
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}
}

HRESULT FlowEngineDX11::Camera::CreateViewport()
{
	m_viewport = { offsetX, offsetY, viewWidth, viewHeight, 0, 1.0 };
	return S_OK;
}

D3D11_VIEWPORT* FlowEngineDX11::Camera::GetViewport()
{
	return &m_viewport;
}

DirectX::XMMATRIX FlowEngineDX11::Camera::CreateViewMatrix()
{
	DirectX::XMVECTOR det;
	return DirectX::XMMatrixInverse(&det, transform->CreateMatrix());
}

DirectX::XMMATRIX FlowEngineDX11::Camera::CreateProjectionMatrix()
{
	if (projectionMode == Orthographic)
	{
		auto projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, 0.0f, farZ);

		return projectionMatrix;
	}
	if (projectionMode == Perspective)
	{
		auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, viewWidth / viewHeight, nearZ, farZ);

		return projectionMatrix;
	}
	return DirectX::XMMatrixIdentity();
}

HRESULT FlowEngineDX11::Camera::CreateBuffers(ID3D11Device* g_device, ID3D11DeviceContext* g_context)
{
	D3D11_BUFFER_DESC cameraBufferDesc{};
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.ByteWidth = sizeof(ViewProjectionBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = g_device->CreateBuffer(&cameraBufferDesc, nullptr, &cameraBuffer);
	if (FAILED(hr)) return hr;

	g_context->VSSetConstantBuffers(1, 1, &cameraBuffer);

	return S_OK;
}

HRESULT FlowEngineDX11::Camera::UpdateBuffers(ID3D11DeviceContext* g_context)
{
	D3D11_MAPPED_SUBRESOURCE mappedCamera;
	HRESULT hr = g_context->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCamera);

	if (SUCCEEDED(hr)) {
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMStoreFloat4x4(
			&viewMatrix,
			DirectX::XMMatrixTranspose(CreateViewMatrix())
		);

		DirectX::XMStoreFloat4x4(
			&projectionMatrix,
			DirectX::XMMatrixTranspose(CreateProjectionMatrix())
		);
		ViewProjectionBuffer cameraMatrix = {viewMatrix, projectionMatrix};

		memcpy(mappedCamera.pData, &cameraMatrix, sizeof(cameraMatrix));

		g_context->Unmap(cameraBuffer, 0);
	}
	else return hr;

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Camera::GetRegistry()
{
	return ObjectRegistry::Camera;
}
