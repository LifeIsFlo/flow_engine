#include "AssetDatabase.h"
#include "DX11Protocol.h"
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
#include "Material.h"
#include "SpriteLoader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <utility>

namespace FlowEngineDX11 
{
	std::map<std::string, std::unique_ptr<Model>> AssetDatabase::_models;
	std::map<std::string, std::unique_ptr<Sprite>> AssetDatabase::_sprites;
	std::map<std::string, std::unique_ptr<VertexShader>> AssetDatabase::_vertexShaders;
	std::map<std::string, std::unique_ptr<PixelShader>> AssetDatabase::_pixelShaders;
	std::map<std::string, std::unique_ptr<Material>> AssetDatabase::_materials;

	Model* AssetDatabase::LoadModel(const std::string& assetLocation)
	{
		flowio::Console::WriteLine("Loading model: " + assetLocation + "...");

		if (!_models.contains(assetLocation)) {
			Model* ret = nullptr;

			if(AssetPathExists(assetLocation))
			if (assetLocation.ends_with(".obj")) {
				ObjLoader::LoadModel(assetLocation, _models);
				ret = _models[assetLocation].get();
			}
			if (ret) flowio::Console::WriteLine("Model loaded!");
			return ret;
		}

		flowio::Console::WriteLine("Model found!");
		return _models[assetLocation].get();
	}

	Sprite* AssetDatabase::LoadSprite(const std::string& assetLocation)
	{
		flowio::Console::WriteLine("Loading sprite: " + assetLocation + "...");
		if (!_sprites.contains(assetLocation))
		{
			if (AssetPathExists(assetLocation)) {

				SpriteLoader::LoadSprite(assetLocation, _sprites);
				Sprite* ret = _sprites[assetLocation].get();

				if (ret) flowio::Console::WriteLine("Sprite loaded!");
				return ret;
			}
			return nullptr;
		}

		flowio::Console::WriteLine("Sprite found!");
		return _sprites[assetLocation].get();
	}

	PixelShader* AssetDatabase::LoadPixelShader(const std::string& assetLocation)
	{
		if (!_pixelShaders.contains(assetLocation)) {
			if (!std::filesystem::exists(assetLocation))
			{
				flowio::Console::WriteLine("Asset file does not exist!: " + assetLocation);
				return nullptr;
			}
			auto ps = std::make_unique<PixelShader>();
			PixelShader* ret = ps.get();
			_pixelShaders[assetLocation] = std::move(ps);
			return ret;
		}

		return _pixelShaders[assetLocation].get();
	}

	VertexShader* AssetDatabase::LoadVertexShader(const std::string& assetLocation, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc)
	{
		if (!_vertexShaders.contains(assetLocation)) {

			auto vs = std::make_unique<VertexShader>(ILDesc);
			VertexShader* ret = vs.get();
			_vertexShaders[assetLocation] = std::move(vs);
			return ret;
		}

		return _vertexShaders[assetLocation].get();
	}

	Material* AssetDatabase::LoadMaterial(const std::string& assetLocation)
	{
		return nullptr;
	}

	HRESULT AssetDatabase::CreateBuffers(const DX11Protocol* dx11_protocol)
	{
		HRESULT hr = S_OK;

		for (auto& model : _models)
		{
			hr = model.second->CreateBuffers(dx11_protocol);
			if (FAILED(hr)) return hr;
		}

		for (auto& sprite : _sprites)
		{
			hr = sprite.second->CreateTexture(dx11_protocol);
			if (FAILED(hr)) return hr;
		}

		for (auto& pixelShader : _pixelShaders) {
			pixelShader.second->CreateShader(std::wstring(pixelShader.first.begin(), pixelShader.first.end()).c_str(), dx11_protocol);
			if (FAILED(hr)) return hr;
		}

		for (auto& vertexShader : _vertexShaders) {
			hr = vertexShader.second->CreateShader(std::wstring(vertexShader.first.begin(), vertexShader.first.end()).c_str(), dx11_protocol);
			if (FAILED(hr)) return hr;
		}

		for (auto& material : _materials) {
			hr = material.second->CreateBuffers(dx11_protocol);
			if (FAILED(hr)) return hr;
		}

		return hr;
	}

	bool AssetDatabase::AssetPathExists(const std::string& assetLocation)
	{
		bool ret = std::filesystem::exists(assetLocation);
		if (!ret)
		{
			flowio::Console::WriteLine("Asset file does not exist!: " + assetLocation);
		}
		return ret;
	}

	void AssetDatabase::CleanUp()
	{
		_models.clear();
		_sprites.clear();
		_vertexShaders.clear();
		_pixelShaders.clear();
		_materials.clear();
	}
}