#include "Renderer.h"

#include <span>
#include "Vertex.h"
#include "Transform.h"
#include "Camera.h"
#include "DirectXMath.h"
#include "TransformBuffer.h"

FlowEngineDX11::Renderer::~Renderer()
{
	if (transformBuffer)
	{
		transformBuffer->Release();
		transformBuffer = nullptr;
	}
}

HRESULT FlowEngineDX11::Renderer::CreateBuffers(const DX11Protocol* dx11_protocol)
{
	auto g_device = dx11_protocol->GetDevice();
	D3D11_BUFFER_DESC transformBufferDesc{};
	transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformBufferDesc.ByteWidth = sizeof(TransformBuffer);
	transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = g_device->CreateBuffer(&transformBufferDesc, nullptr, &transformBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void FlowEngineDX11::Renderer::UpdateBuffers(const DX11Protocol* dx11_protocol)
{
	UpdateTransformBuffer(dx11_protocol);
}

HRESULT FlowEngineDX11::Renderer::UpdateTransformBuffer(const DX11Protocol* dx11_protocol)
{
	auto g_context = dx11_protocol->GetContext();
	D3D11_MAPPED_SUBRESOURCE mappedTransform;
	HRESULT hr = g_context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTransform);

	if (SUCCEEDED(hr)) {
		DirectX::XMFLOAT4X4 transformMatrix;
		DirectX::XMFLOAT4X4 rotationMatrix;
		DirectX::XMStoreFloat4x4(&transformMatrix, DirectX::XMMatrixTranspose(
			transform->CreateMatrix()
		));
		auto xRot = DirectX::XMMatrixRotationX(transform->rotation.x);
		auto yRot = DirectX::XMMatrixRotationY(transform->rotation.y);
		auto zRot = DirectX::XMMatrixRotationZ(transform->rotation.z);
		DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixTranspose(
			xRot * yRot * zRot
		));

		TransformBuffer buff{};
		buff.rotationMatrix = rotationMatrix;
		buff.transformMatrix = transformMatrix;

		memcpy(mappedTransform.pData, &buff, sizeof(buff));

		g_context->Unmap(transformBuffer, 0);
	}
	else return hr;

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Renderer::GetRegistry()
{
	return ObjectRegistry::Renderer;
}
