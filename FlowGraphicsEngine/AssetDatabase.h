#pragma once
#include <map>
#include <memory>
#include <string>
#include "Mesh.h" 
#include "FWD.h"

namespace FlowEngineDX11
{
	class AssetDatabase
	{
		static std::map<std::string, std::unique_ptr<Model>> _models;
		static std::map<std::string, std::unique_ptr<Sprite>> _sprites;
	public:
		static Model* LoadModel(const std::string& assetLocation);
		static Sprite* LoadSprite(const std::string& assetLocation);
		static void CleanUp();
		static void ListModels();
	};
}

