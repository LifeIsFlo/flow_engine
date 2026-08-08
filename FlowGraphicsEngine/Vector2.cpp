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
