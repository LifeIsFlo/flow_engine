#pragma once
#include "Renderer.h"
#include "FWD.h"

namespace FlowEngineDX11 {
    class SpriteRenderer :
        public Renderer
    {
    public:
        Sprite* sprite;
		HRESULT CreateResources(ID3D11DeviceContext* g_context, ID3D11Device* g_device) override;
    protected:
		void Render(ID3D11DeviceContext* g_context, ID3D11Device* g_device, Camera* camera) override;
    };
}
