#pragma once
#include <d3d11.h>

#include "FWD.h"

namespace FlowEngineDX11
{
	class RenderProtocol
	{
		std::vector<Renderer*> renderers;
	public:
		void Render(const DX11Protocol* dx11_protocol);
		void UpdateBuffers(const DX11Protocol* dx11_protocol);
		HRESULT CreateResources(const DX11Protocol* dx11_protocol);
		Renderer* AddRenderer(Renderer* renderer);
		void RemoveRenderer(Renderer* renderer);
		void ClearState();
	};
}
