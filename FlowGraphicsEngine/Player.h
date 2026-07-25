#pragma once
#include "Renderer.h"

class Player : public FlowEngine::Renderer
{
public:
    float speed;
    bool wasd;

    void Update(float fElapsedTime) override;
    void Render(FlowEngine::Engine* engine) override;
};

