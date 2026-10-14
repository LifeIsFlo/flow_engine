#include "RenderProtocol.h"
#include "Renderer.h"

FlowEngineDX11::Renderer* FlowEngineDX11::RenderProtocol::AddRenderer(Renderer* renderer)
{
    renderers.push_back(renderer);
    return renderer;
}
void FlowEngineDX11::RenderProtocol::RemoveRenderer(Renderer* renderer)
{
    std::erase_if(renderers, [renderer](const Renderer* element)
    {
        return element == renderer;
    });
}

void FlowEngineDX11::RenderProtocol::ClearState()
{
    renderers.clear();
    renderers.shrink_to_fit();
}

void FlowEngineDX11::RenderProtocol::Render(const DX11Protocol* dx11_protocol)
{
    for (Renderer* renderer : renderers)
    {
        renderer->PrepareMainPass(dx11_protocol);
        renderer->Render(dx11_protocol);
    }
}

void FlowEngineDX11::RenderProtocol::UpdateBuffers(const DX11Protocol* dx11_protocol)
{
    for (Renderer* renderer : renderers) 
    {
        renderer->UpdateBuffers(dx11_protocol);
    }
}

HRESULT FlowEngineDX11::RenderProtocol::CreateResources(const DX11Protocol* dx11_protocol)
{
    for (Renderer* renderer : renderers)
    {
        HRESULT hr = renderer->CreateBuffers(dx11_protocol);
        if (FAILED(hr)) return hr;
    }

    return S_OK;
}
