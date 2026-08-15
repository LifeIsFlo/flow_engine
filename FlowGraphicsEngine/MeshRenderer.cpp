#include "MeshRenderer.h"

#include "Mesh.h"
#include "Sprite.h"
#include "Vertex.h"

FlowEngineDX11::MeshRenderer::~MeshRenderer()
{
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

void FlowEngineDX11::MeshRenderer::Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera)
{
	g_context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

HRESULT FlowEngineDX11::MeshRenderer::CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	HRESULT hr = CreateBuffers(g_device, mesh->GetIndices(),mesh->GetVertices());
	if (FAILED(hr)) return hr;

	return S_OK;
}
