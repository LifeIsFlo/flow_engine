#pragma once
#include "flow_fwd.h"

namespace flow
{
    struct alignas(16) float4x4a
    {
        float data[4][4];

        static float4x4a identity()
        {
            return {{{1, 0, 0, 0},
                     {0, 1, 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1}}};
        }
    };
}