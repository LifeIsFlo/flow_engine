#pragma once
#include <map>
#include <memory>
#include <string>
#include "FWD.h"

namespace FlowEngineDX11
{
	class AssetDatabase
	{
		static std::map<std::string, std::unique_ptr<Model>> _models;
		static std::map<std::string, std::unique_ptr<Sprite>> _sprites;
		static std::map<std::string, std::unique_ptr<VertexShader>> _vertexShaders;
		static std::map<std::string, std::unique_ptr<PixelShader>> _pixelShaders;
		static std::map<std::string, std::unique_ptr<Material>> _materials;
	public:
		static Model* LoadModel(const std::string& assetLocation);
		static PixelShader* LoadPixelShader(const std::string& assetLocation);
		static VertexShader* LoadVertexShader(const std::string& assetLocation, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc);
		static Material* LoadMaterial(const std::string& assetLocation);
		static Sprite* LoadSprite(const std::string& assetLocation);
		static HRESULT CreateBuffers(const DX11Protocol* dx11_protocol);

		static void CleanUp();

		static bool AssetPathExists(const std::string& assetLocation);

		template<typename T>
		requires std::derived_from<T, Object>
		static bool AssetExistsAndLoaded(const std::string& assetLocation)
		{
			return AssetPathExists(assetLocation) && AssetRegistry<T>::get().contains(assetLocation);
		}

		template<typename T>
		struct AssetRegistry;

		template<>
		struct AssetRegistry<Model> {
			static auto& get() { return _models; }
		};

		template<>
		struct AssetRegistry<Sprite> {
			static auto& get() { return _sprites; }
		};

		template<>
		struct AssetRegistry<VertexShader> {
			static auto& get() { return _vertexShaders; }
		};

		template<>
		struct AssetRegistry<PixelShader> {
			static auto& get() { return _pixelShaders; }
		};

		template<>
		struct AssetRegistry<Material> {
			static auto& get() { return _materials; }
		};
	};
}

