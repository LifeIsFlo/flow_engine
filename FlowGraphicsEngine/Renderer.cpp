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

HRESULT FlowEngineDX11::Renderer::CreateBuffers(ID3D11Device* g_device)
{
	D3D11_BUFFER_DESC transformBufferDesc{};
	transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformBufferDesc.ByteWidth = sizeof(TransformBuffer);
	transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = g_device->CreateBuffer(&transformBufferDesc, nullptr, &transformBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void FlowEngineDX11::Renderer::UpdateAndRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	UpdateTransformBuffer(g_context, g_device, camera);
	g_context->VSSetConstantBuffers(0, 1, &transformBuffer);
	Render(g_context, g_device, camera);
}

HRESULT FlowEngineDX11::Renderer::UpdateTransformBuffer(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	D3D11_MAPPED_SUBRESOURCE mappedTransform;
	HRESULT hr = g_context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTransform);

	if (SUCCEEDED(hr)) {
		TransformBuffer transformMatrix = {};

		memcpy(mappedTransform.pData, &transformMatrix, sizeof(transformMatrix));

		g_context->Unmap(transformBuffer, 0);
	}
	else return hr;

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Renderer::GetRegistry()
{
	return ObjectRegistry::Renderer;
}
