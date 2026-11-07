#pragma once
#include "flow_data_registry.h"
#include "flow_dx11_pixel_shader.h"
#include "flow_dx11_vertex_shader.h"

namespace flow::win32
{
    class flow_dx11_shader_database
    {
        DataRegistry<DX11VertexShader> _vertexShaders;
        DataRegistry<DX11PixelShader> _pixelShaders;

    public:
        void InitializeShaders(ID3D11Device* g_device);
        DX11PixelShader* LoadDX11PixelShader(const std::string& assetLocation);
        DX11VertexShader* LoadDX11VertexShader(const std::string& assetLocation, const std::vector<D3D11_INPUT_ELEMENT_DESC>& ILDesc);
    };
}