#include "SpriteRenderer.h"
#include "Vertex.h"
#include "Sprite.h"

void FlowEngineDX11::SpriteRenderer::Render(const DX11Protocol* dx11_protocol)
{
	auto g_context = dx11_protocol->GetContext();
	g_context->PSSetShaderResources(0, 1, sprite->GetTextureResourceAddress());
	g_context->DrawIndexed(6, 0, 0);
}

void FlowEngineDX11::SpriteRenderer::PrepareMainPass(const DX11Protocol* dx11_protocol)
{
}
