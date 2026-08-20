#include "SpriteRenderer.h"
#include "Vertex.h"
#include "Sprite.h"

HRESULT FlowEngineDX11::SpriteRenderer::CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	HRESULT hr = sprite->CreateTexture(g_device);
	if (FAILED(hr)) return hr;

	/*
	Vertex quad[] =
	{
		{-0.5f,  0.5f, 0, 0, 0, 0}, // 0
		{ 0.5f,  0.5f, 0, 1, 0, 0}, // 1
		{-0.5f, -0.5f, 0, 0, 1, 0}, // 2
		{ 0.5f, -0.5f, 0, 1, 1, 0}  // 3
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,1,3
	};
	*/

	hr = CreateBuffers(g_device);
	if (FAILED(hr)) return hr;


	return S_OK;
}

void FlowEngineDX11::SpriteRenderer::Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	g_context->PSSetShaderResources(0, 1, sprite->GetTextureResourceAddress());
	g_context->DrawIndexed(6, 0, 0);
}