#pragma once
#include "FWD.h"
#include "Vector3.h"
#include "Color.h"

namespace FlowEngineDX11
{
    struct LightData
    {
        Vector3 position{};
        float radius = 0;

        GPUColor color{};

        float intensity = 0;
        BOOL fallOffEnabled = FALSE;
        Vector2 pad1{};

        constexpr LightData(const Vector3& _position, const float& _radius, const GPUColor& _color, const float& _intensity, const bool& _falloffEnabled) noexcept
            : position(_position), radius(_radius), color(_color), intensity(_intensity), fallOffEnabled(_falloffEnabled) {}
    };
}
