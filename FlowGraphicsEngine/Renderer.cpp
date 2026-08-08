#include "Renderer.h"

#include <span>

#include "Transform.h"
#include "Camera.h"
#include "DirectXMath.h"



FlowEngineDX11::Renderer::~Renderer()
{
	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	if (transformBuffer)
	{
		transformBuffer->Release();
		transformBuffer = nullptr;
	}
	std::cout << "Renderer Released!";
}

void FlowEngineDX11::Renderer::UpdateAndRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	UpdateTransformBuffer(g_context, g_device, camera);
	Render(g_context, g_device, camera);
}

HRESULT FlowEngineDX11::Renderer::CreateBuffers2D(ID3D11Device* g_device, std::span<const unsigned int> indices, std::span<const Vertex2D> vertices)
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertices.size() * sizeof(Vertex2D);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = vertices.data();

	HRESULT hr = g_device->CreateBuffer(&vertexBufferDesc, &vInitData, &vertexBuffer);
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = indices.size() * sizeof(int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iInitData{};
	iInitData.pSysMem = indices.data();

	hr = g_device->CreateBuffer(&indexBufferDesc, &iInitData, &indexBuffer);
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC transformBufferDesc{};
	transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
	transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = g_device->CreateBuffer(&transformBufferDesc, nullptr, &transformBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

DirectX::XMFLOAT4X4 FlowEngineDX11::Renderer::CreateGPUMatrix(Camera* camera)
{
	DirectX::XMFLOAT4X4 gpuMatrix;
	DirectX::XMStoreFloat4x4(&gpuMatrix, DirectX::XMMatrixTranspose(
		transform->CreateMatrix() *
		camera->CreateViewProjectionMatrix()
		));

	return gpuMatrix;
}

HRESULT FlowEngineDX11::Renderer::UpdateTransformBuffer(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = g_context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	if (SUCCEEDED(hr)) {
		auto transformMatrix = CreateGPUMatrix(camera);

		memcpy(mapped.pData, &transformMatrix, sizeof(transformMatrix));

		g_context->Unmap(transformBuffer, 0);
	}
	else return hr;

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Renderer::GetRegistry()
{
	return ObjectRegistry::Renderer;
}
