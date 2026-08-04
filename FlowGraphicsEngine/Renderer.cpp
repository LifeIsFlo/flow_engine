#include "Renderer.h"
#include "Transform.h"

void FlowEngineDX11::Renderer::OnRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	
}

void FlowEngineDX11::Renderer::Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	UploadTexture(g_context);
	g_context->PSSetShaderResources(0, 1, &texture2dSRV);
	UINT stride = sizeof(Vertex2D);
	UINT offset = 0;
	g_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	g_context->DrawIndexed(4, 0, 0);
	OnRender(g_context, g_device);
}

HRESULT FlowEngineDX11::Renderer::CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	HRESULT hr = CreateTexture(g_device);
	if (FAILED(hr)) return hr;

	hr = CreateBuffers(g_device);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT FlowEngineDX11::Renderer::CreateTexture(ID3D11Device* g_device)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = static_cast<UINT>(GetWidth());
	desc.Height = static_cast<UINT>(GetHeight());
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = g_device->CreateTexture2D(&desc, nullptr, &texture2d);
	if (FAILED(hr)) return hr;

	hr = g_device->CreateShaderResourceView(texture2d.Get(), nullptr, &texture2dSRV);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT FlowEngineDX11::Renderer::UploadTexture(ID3D11DeviceContext* g_context)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = g_context->Map(
		texture2d.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD, // Discards last frame's data so GPU doesn't stall
		0,
		&mappedResource
	);

	if (SUCCEEDED(hr)) {
		auto colors = GetColorData();
		BYTE* gpuMemory = static_cast<BYTE*>(mappedResource.pData);
		BYTE* cpuMemory = reinterpret_cast<BYTE*>(colors.data());

		size_t byteWidthCount = GetWidth() * sizeof(Color);

		// Row-by-row copy handles potential GPU memory alignment padding securely
		for (UINT row = 0; row < GetHeight(); ++row) {
			memcpy(
				gpuMemory + (row * mappedResource.RowPitch),
				cpuMemory + (row * byteWidthCount),
				byteWidthCount
			);
		}
			
		g_context->Unmap(texture2d.Get(), 0);
	}
	else
	{
		return hr;
	}

	return S_OK;
}

HRESULT FlowEngineDX11::Renderer::CreateBuffers(ID3D11Device* g_device)
{
	Vertex2D quad[] = {
{-1,  1, 0,0},
{ 1,  1, 1, 0},
{-1, -1, 1, 1},
{ 1, -1, 0, 1},
	};

	unsigned int indices[] = {
		0,1,2, 0,2,3
	};

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(quad);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = quad;

	HRESULT hr = g_device->CreateBuffer(&vertexBufferDesc, &vInitData, &vertexBuffer);
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iInitData{};
	iInitData.pSysMem = indices;

	hr = g_device->CreateBuffer(&indexBufferDesc, &iInitData, &indexBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Renderer::GetRegistry()
{
	return ObjectRegistry::Renderer;
}
