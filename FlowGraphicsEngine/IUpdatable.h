#pragma once
#include "Engine.h"

namespace FlowEngine {
    class IUpdatable {
    public:
        virtual ~IUpdatable() = default;
        virtual void Update(float deltaTime) = 0;
        virtual void FixedUpdate(float fixedTick) = 0;
    };
}