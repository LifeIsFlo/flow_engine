#pragma once
#include "Behaviour.h"
#include "FlowEngine.h"
#include "Object.h"
#include "Component.h"
class PlayerMovement : public FlowEngineDX11::Behaviour
{
public:
    float speed = 5;
    bool wasd = true;
    void Update() override;
};