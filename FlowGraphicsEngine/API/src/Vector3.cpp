#include "Vector3.h"

const FlowEngineDX11::Vector3 FlowEngineDX11::Vector3::ZERO = {0,0,0};
const FlowEngineDX11::Vector3 FlowEngineDX11::Vector3::ONE = {1,1,1};

const FlowEngineDX11::Vector3 FlowEngineDX11::Vector3::FORWARD = { 0,0,1 };
const FlowEngineDX11::Vector3 FlowEngineDX11::Vector3::UP { 0,1,0 };
const FlowEngineDX11::Vector3 FlowEngineDX11::Vector3::RIGHT { 1,0,0 };

float FlowEngineDX11::Vector3::mag()
{
    return std::sqrt(x * x + y * y + z * z);
}

void FlowEngineDX11::Vector3::normalize()
{
	float magnitude = mag();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}
