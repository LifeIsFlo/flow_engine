#pragma once
#include "FWD.h"
#include "Renderer.h"
#include <d3d11.h>

namespace FlowEngineDX11
{
	class MeshRenderer : public Renderer
	{
	public:
		Mesh* mesh = nullptr;
		~MeshRenderer();
	protected:
		void Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera) override;
	public:
		HRESULT CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device) override;
	};
}
