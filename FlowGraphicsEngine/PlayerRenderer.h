#pragma once
#include "Renderer.h"
#include "DX11Protocol.h"

class PlayerRenderer : public FlowEngineDX11::Renderer
{
public:
	void OnRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device) override;
};
