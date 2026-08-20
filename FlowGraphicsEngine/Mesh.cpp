#include "Mesh.h"
#include "Vertex.h"

FlowEngineDX11::Mesh::Mesh(std::span<const unsigned int> indices, std::span<const Vertex> vertices)
{
	this->m_indices.insert(m_indices.begin(), indices.begin(), indices.end());
	this->m_vertices.insert(m_vertices.begin(),vertices.begin(), vertices.end());

	m_indices.shrink_to_fit();
	m_vertices.shrink_to_fit();
}

FlowEngineDX11::Mesh::~Mesh()
{
	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

HRESULT FlowEngineDX11::Mesh::CreateBuffers(ID3D11Device* g_device)
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_vertices.size() * sizeof(Vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = m_vertices.data();

	HRESULT hr = g_device->CreateBuffer(&vertexBufferDesc, &vInitData, &vertexBuffer);
	if (FAILED(hr)) return hr;

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indices.size() * sizeof(int);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iInitData{};
	iInitData.pSysMem = m_indices.data();

	hr = g_device->CreateBuffer(&indexBufferDesc, &iInitData, &indexBuffer);
	if (FAILED(hr)) return hr;

	return S_OK;
}

ID3D11Buffer* FlowEngineDX11::Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

ID3D11Buffer* FlowEngineDX11::Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer** FlowEngineDX11::Mesh::GetIndexBufferAddress()
{
	return &indexBuffer;
}

ID3D11Buffer** FlowEngineDX11::Mesh::GetVertexBufferAddress()
{
	return &vertexBuffer;
}

std::span<const FlowEngineDX11::Vertex> FlowEngineDX11::Mesh::GetVertices()
{
	return m_vertices;
}

std::span<const unsigned int> FlowEngineDX11::Mesh::GetIndices()
{
	return m_indices;
}

unsigned int FlowEngineDX11::Mesh::GetIndexCount() const
{
	return m_indices.size();
}


