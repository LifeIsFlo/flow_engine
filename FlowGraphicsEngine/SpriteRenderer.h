#pragma once
#include "Renderer.h"
#include "FWD.h"

namespace FlowEngineDX11 {
    class SpriteRenderer :
        public Renderer
    {
    public:
        Sprite* sprite;
		void Render(const DX11Protocol* dx11_protocol) override;
        void PrepareMainPass(const DX11Protocol* dx11_protocol) override;
    };
}
