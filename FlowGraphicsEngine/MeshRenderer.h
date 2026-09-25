#pragma once
#include "FWD.h"
#include "Renderer.h"

namespace FlowEngineDX11
{
	class MeshRenderer : public Renderer
	{
	public:
		Mesh* mesh = nullptr;
		Sprite* sprite = nullptr;
		void Render(const DX11Protocol* dx11_protocol) override;
		void PrepareMainPass(const DX11Protocol* dx11_protocol) override;
	};
}
