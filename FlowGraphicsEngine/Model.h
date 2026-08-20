#pragma once
#include <map>
#include <span>

#include "FWD.h"
#include "Object.h"

namespace FlowEngineDX11 {
    class Model : public Object
    {
    private:
        std::map<std::string, std::unique_ptr<Mesh>> meshes;
    public:
        void ListMeshes();
        Mesh* GetMesh(const std::string& meshName);
        void AddMesh(const std::string& meshName, const std::span<const unsigned int>& indices, const std::span<const Vertex>& vertices);
    };
}
