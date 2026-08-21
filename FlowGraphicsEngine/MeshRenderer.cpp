#include "MeshRenderer.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Vertex.h"

void FlowEngineDX11::MeshRenderer::Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11ShaderResourceView* nullSRV = nullptr;
	if (sprite) g_context->PSSetShaderResources(0, 1, sprite->GetTextureResourceAddress());
	else g_context->PSSetShaderResources(0, 1, &nullSRV);
	g_context->IASetVertexBuffers(0, 1, mesh->GetVertexBufferAddress(), &stride, &offset);
	g_context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, offset);
	g_context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

HRESULT FlowEngineDX11::MeshRenderer::CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	HRESULT hr = CreateBuffers(g_device, g_context);
	if (FAILED(hr)) return hr;

	hr = mesh->CreateBuffers(g_device);
	if (FAILED(hr)) return hr;

	if (sprite) {
		hr = sprite->CreateTexture(g_device);
		if (FAILED(hr)) return hr;
	}



	return S_OK;
}
