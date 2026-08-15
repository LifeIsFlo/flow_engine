#pragma once
#include <map>
#include "FWD.h"

namespace FlowEngineDX11
{
	class AssetDatabase
	{
		std::map<std::string, std::unique_ptr<Mesh>> _meshes;
		std::map<std::string, std::unique_ptr<Sprite>> _sprites;
	public:
		static Mesh* LoadMesh(std::string assetLocation);
	};
}

