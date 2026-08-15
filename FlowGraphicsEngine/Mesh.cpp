#include "Mesh.h"
#include "Vertex.h"

FlowEngineDX11::Mesh::Mesh(std::span<const unsigned int> indices, std::span<const Vertex> vertices)
{
	this->m_indices.insert(m_indices.begin(), indices.begin(), indices.end());
	this->m_vertices.insert(m_vertices.begin(),vertices.begin(), vertices.end());

	m_indices.shrink_to_fit();
	m_vertices.shrink_to_fit();
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

FlowEngineDX11::Mesh FlowEngineDX11::Mesh::Cube()
{
	Vertex cube[] = {
	{-1.0f,  1.0f, -1.0f, 0, 0, 0},
	{ 1.0f,  1.0f, -1.0f, 1, 0, 0},
	{-1.0f, -1.0f, -1.0f, 0, 1, 0},
	{ 1.0f, -1.0f, -1.0f, 1, 1, 0},
	{-1.0f,  1.0f, 1.0f, 1, 1, 0},
	{ 1.0f,  1.0f, 1.0f, 0, 1, 0},
	{-1.0f, -1.0f, 1.0f, 1, 0, 0},
	{ 1.0f, -1.0f, 1.0f, 0, 0, 0}
	};

	unsigned int indices[] = {
		0,1,2,
		2,1,3,
		7,5,6,
		6,5,4,
		7,6,3,
		6,2,3,
		7,3,1,
		5,7,1,
		6,0,2,
		4,0,6,
		5,0,4,
		0,5,1
	};

    return Mesh(indices, cube);
}


