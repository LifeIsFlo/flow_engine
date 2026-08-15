#pragma once
#include <span>
#include "FWD.h"
#include "Vertex.h"

namespace FlowEngineDX11
{
	class Mesh
	{
	private:
		std::vector<unsigned int> m_indices;
		std::vector<Vertex> m_vertices;
	public:
		Mesh(std::span<const unsigned int> indices, std::span<const Vertex> vertices);
		std::span<const Vertex> GetVertices();
		std::span<const unsigned int> GetIndices();
		unsigned int GetIndexCount() const;
		static Mesh Cube();
	};
}

