#include "AssetDatabase.h"
#include "ObjLoader.h"
#include "Vector3.h"
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include "Model.h"
#include <filesystem>
#include "Console.hpp"
#include "Mesh.h"
#include "Sprite.h"
#include "Util.h"
#include <utility>

std::map<std::string, std::unique_ptr<FlowEngineDX11::Model>> FlowEngineDX11::AssetDatabase::_models;
std::map<std::string, std::unique_ptr<FlowEngineDX11::Sprite>> FlowEngineDX11::AssetDatabase::_sprites;

FlowEngineDX11::Model* FlowEngineDX11::AssetDatabase::LoadModel(const std::string& assetLocation)
{
	flowio::Console::WriteLine("Loading model: " + assetLocation +"...");

	if (!_models.contains(assetLocation)) {
		Model* ret = nullptr;

		if (assetLocation.ends_with(".obj")) {
			std::ifstream modelFile(assetLocation);
			if (!modelFile.is_open())
			{
				flowio::Console::WriteLine("Asset file does not exist!: " + assetLocation);
				return nullptr;
			}
			ObjLoader::LoadModel(modelFile, &_models, assetLocation);
			ret = _models[assetLocation].get();
		}
		if (ret != nullptr) flowio::Console::WriteLine("Model loaded!");
		return ret;
	}

	flowio::Console::WriteLine("Model found!");
	return _models[assetLocation].get();
}

FlowEngineDX11::Sprite* FlowEngineDX11::AssetDatabase::LoadSprite(const std::string& assetLocation)
{
	flowio::Console::WriteLine("Loading sprite: " + assetLocation + "...");
	if (!_sprites.contains(assetLocation))
	{
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>(500, 500);
		Sprite* ret = sprite.get();
		_sprites[assetLocation] = std::move(sprite);

		flowio::Console::WriteLine("Sprite loaded!");
		return ret;
	}

	flowio::Console::WriteLine("Sprite loaded!");
	return _sprites[assetLocation].get();
}

void FlowEngineDX11::AssetDatabase::CleanUp()
{
	_models.clear();
	_sprites.clear();
}

void FlowEngineDX11::AssetDatabase::ListModels()
{
	for (auto& model : _models)
	{
		flowio::Console::Write("model: ");
		flowio::Console::WriteLine(model.first);
		model.second->ListMeshes();
	}
}
