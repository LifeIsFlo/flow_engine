#include "FWD.h"
#include "Vector2.h"

float FlowEngineDX11::Vector2::Distance(Vector2 first, Vector2 second)
{
    return (first - second).Magnitude();
}

float FlowEngineDX11::Vector2::Magnitude()
{
    return std::sqrt(x * x + y * y);
}

const FlowEngineDX11::Vector2 FlowEngineDX11::Vector2::ZERO = { 0,0 };
const FlowEngineDX11::Vector2 FlowEngineDX11::Vector2::ONE = { 1,1 };