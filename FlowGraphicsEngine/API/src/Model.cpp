#include "Model.h"
#include <memory>
#include "Console.hpp"
#include "Mesh.h"

FlowEngineDX11::Mesh* FlowEngineDX11::Model::GetMesh(const std::string& meshName)
{
	if (!meshes.contains(meshName)) return nullptr;
	return meshes[meshName].get();
}

void FlowEngineDX11::Model::AddMesh(const std::string& meshName, const std::span<const unsigned int>& indices, const std::span<const Vertex>& vertices)
{
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(indices, vertices);
	mesh->name = meshName;
	meshes[meshName] = std::move(mesh);
}

HRESULT FlowEngineDX11::Model::CreateBuffers(const DX11Protocol* dx11_protocol)
{
	HRESULT hr = S_OK;
	for (auto& mesh : meshes)
	{
		hr = mesh.second->CreateBuffers(dx11_protocol);
		if (FAILED(hr)) return hr;
	}

	return hr;
}