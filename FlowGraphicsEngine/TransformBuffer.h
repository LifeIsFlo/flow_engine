#pragma once
#include "FWD.h"
#include "Vector3.h"

namespace FlowEngineDX11 
{
	struct TransformBuffer 
	{
		DirectX::XMFLOAT4X4 transformMatrix;
		DirectX::XMFLOAT4X4 rotationMatrix;
	};
}