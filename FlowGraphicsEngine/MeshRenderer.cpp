#include "MeshRenderer.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Vertex.h"

void FlowEngineDX11::MeshRenderer::Render(const DX11Protocol* dx11_protocol)
{
	if (mesh) {
		auto g_context = dx11_protocol->GetContext();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		g_context->VSSetConstantBuffers(0, 1, &transformBuffer);
		g_context->IASetVertexBuffers(0, 1, mesh->GetVertexBufferAddress(), &stride, &offset);
		g_context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, offset);
		g_context->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
	}
}

void FlowEngineDX11::MeshRenderer::PrepareMainPass(const DX11Protocol* dx11_protocol)
{
	auto g_context = dx11_protocol->GetContext();
	if (sprite) 
	{
		g_context->PSSetShaderResources(0, 1, sprite->GetTextureResourceAddress());
	}
	else {
		ID3D11ShaderResourceView* nullSRV = nullptr;
		g_context->PSSetShaderResources(0, 1, &nullSRV);
	}
}
