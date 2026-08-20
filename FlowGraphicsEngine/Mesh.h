#pragma once
#include <d3d11.h>
#include <span>
#include "FWD.h"
#include "Object.h"
#include "Vertex.h"

namespace FlowEngineDX11
{
	class Mesh : public Object
	{
	private:
		ID3D11Buffer* indexBuffer = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;
		std::vector<unsigned int> m_indices;
		std::vector<Vertex> m_vertices;
	public:
		Mesh(std::span<const unsigned int> indices, std::span<const Vertex> vertices);
		~Mesh() override;
		HRESULT CreateBuffers(ID3D11Device* g_device);
		ID3D11Buffer* GetIndexBuffer();
		ID3D11Buffer* GetVertexBuffer();
		ID3D11Buffer** GetIndexBufferAddress();
		ID3D11Buffer** GetVertexBufferAddress();
		std::span<const Vertex> GetVertices();
		std::span<const unsigned int> GetIndices();
		unsigned int GetIndexCount() const;
	};
}

