#include "SpriteRenderer.h"
#include "Sprite.h"


HRESULT FlowEngineDX11::SpriteRenderer::CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	HRESULT hr = sprite->CreateTexture(g_device);
	if (FAILED(hr)) return hr;

	Vertex2D quad[] =
	{
		{-200.0f,  200.0f, 0, 0}, // 0
		{ 200.0f,  200.0f, 1, 0}, // 1
		{-200.0f, -200.0f, 0, 1}, // 2
		{ 200.0f, -200.0f, 1, 1}  // 3
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,1,3
	};

	hr = CreateBuffers2D(g_device, indices, quad);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void FlowEngineDX11::SpriteRenderer::Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	sprite->UploadTexture(g_context);
	g_context->PSSetShaderResources(0, 1, sprite->GetTextureResourceAddress());
	UINT stride = sizeof(Vertex2D);
	UINT offset = 0;
	g_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	g_context->VSSetConstantBuffers(0, 1, &transformBuffer);
	g_context->DrawIndexed(6, 0, 0);
}

FlowEngineDX11::SpriteRenderer::~SpriteRenderer()
{
	if (sprite) delete sprite;
	std::cout << "SpriteRenderer released";
}