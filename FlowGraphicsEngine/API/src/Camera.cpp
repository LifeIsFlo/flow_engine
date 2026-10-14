#include "Camera.h"
#include "Transform.h"
#include <DirectXMathMatrix.inl>
#include "Color.h"
#include "DX11Protocol.h"
#include "ViewProjectionBuffer.h"

FlowEngineDX11::Camera::~Camera()
{
	if (cameraBuffer) {
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}
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
		auto projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth / 30, viewHeight / 30, 0.0f, farZ);

		return projectionMatrix;
	}
	if (projectionMode == Perspective)
	{
		auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, viewWidth / viewHeight, nearZ, farZ);

		return projectionMatrix;
	}
	return DirectX::XMMatrixIdentity();
}

HRESULT FlowEngineDX11::Camera::CreateBuffers(const DX11Protocol* dx11_protocol)
{
	auto g_device = dx11_protocol->GetDevice();
	auto g_context = dx11_protocol->GetContext();

	D3D11_BUFFER_DESC cameraBufferDesc{};
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.ByteWidth = sizeof(ViewProjectionBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = g_device->CreateBuffer(&cameraBufferDesc, nullptr, &cameraBuffer);
	if (FAILED(hr)) return hr;

	m_viewport = { offsetX, offsetY, viewWidth, viewHeight, 0.0f, 1.0f };

	return S_OK;
}

HRESULT FlowEngineDX11::Camera::UpdateBuffers(const DX11Protocol* dx11_protocol)
{
	auto g_context = dx11_protocol->GetContext();

	D3D11_MAPPED_SUBRESOURCE mappedCamera;
	HRESULT hr = g_context->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCamera);

	if (SUCCEEDED(hr)) {
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMStoreFloat4x4(
			&viewMatrix,
			DirectX::XMMatrixTranspose(CreateViewMatrix())
		);

		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMStoreFloat4x4(
			&projectionMatrix,
			DirectX::XMMatrixTranspose(CreateProjectionMatrix())
		);
		ViewProjectionBuffer cameraMatrix = {viewMatrix, projectionMatrix, transform->position, 0.0f};

		memcpy(mappedCamera.pData, &cameraMatrix, sizeof(cameraMatrix));

		g_context->Unmap(cameraBuffer, 0);
	}
	else return hr;

	g_context->VSSetConstantBuffers(1, 1, &cameraBuffer);

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Camera::GetRegistry()
{
	return ObjectRegistry::Camera;
}

void FlowEngineDX11::Camera::ApplyChanges(DX11Protocol* dx11_protocol)
{
	auto g_context = dx11_protocol->GetContext();
	auto g_device = dx11_protocol->GetDevice();
	auto g_swapChain = dx11_protocol->GetSwapChain();

	m_viewport = { offsetX, offsetY, viewWidth, viewHeight, 0, 1.0 };
}

// Getters
D3D11_VIEWPORT* FlowEngineDX11::Camera::GetViewport()
{
	return &m_viewport;
}