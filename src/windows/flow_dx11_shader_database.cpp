#include "flow_dx11_shader_database.h"
#include "flow_dxutil.h"
#include "flow_io.h"
#include <filesystem>

namespace flow::win32
{
    void flow_dx11_shader_database::InitializeShaders(ID3D11Device* g_device)
    {
        for (auto& ps : _pixelShaders)
        {
            LPCWSTR wPath = reinterpret_cast<LPCWSTR>(ps.first.c_str());
            ps.second->CreateShader(wPath, g_device);
        }
        for (auto& vs : _vertexShaders)
        {
            LPCWSTR wPath = reinterpret_cast<LPCWSTR>(vs.first.c_str());
            vs.second->CreateShader(wPath, g_device);
        }
    }

    DX11PixelShader* flow_dx11_shader_database::LoadDX11PixelShader(const std::string& assetLocation)
    {
        if (!_pixelShaders.contains(assetLocation))
        {
            if (!std::filesystem::exists(assetLocation))
            {
                flow::io::Console::WriteLine("Asset file does not exist!: " + assetLocation);
                return nullptr;
            }
            auto ps = std::make_unique<DX11PixelShader>();
            DX11PixelShader* ret = ps.get();
            _pixelShaders[assetLocation] = std::move(ps);
            return ret;
        }

        return _pixelShaders[assetLocation].get();
    }
    DX11VertexShader* flow_dx11_shader_database::LoadDX11VertexShader(const std::string& assetLocation, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc)
    {
        if (!_pixelShaders.contains(assetLocation))
        {
            if (!std::filesystem::exists(assetLocation))
            {
                flow::io::Console::WriteLine("Asset file does not exist!: " + assetLocation);
                return nullptr;
            }
            auto ps = std::make_unique<DX11VertexShader>(ILDesc);
            DX11VertexShader* ret = ps.get();
            _vertexShaders[assetLocation] = std::move(ps);
            return ret;
        }

        return _vertexShaders[assetLocation].get();
    }
}