#include "Model.h"
#include <memory>
#include "Console.hpp"
#include "Mesh.h"

void FlowEngineDX11::Model::ListMeshes()
{
	for (auto& mesh: meshes)
	{
		flowio::Console::Write("Object: ");
		flowio::Console::WriteLine(mesh.first);
	}
}

FlowEngineDX11::Mesh* FlowEngineDX11::Model::GetMesh(const std::string& meshName)
{
	return meshes[meshName].get();
}

void FlowEngineDX11::Model::AddMesh(const std::string& meshName, const std::span<const unsigned int>& indices, const std::span<const Vertex>& vertices)
{
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(indices, vertices);
	mesh->name = meshName;
	meshes[meshName] = std::move(mesh);
}
