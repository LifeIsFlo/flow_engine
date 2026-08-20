#pragma once
#include <DirectXMath.h>

#include "FWD.h"
#include "Vector3.h"

namespace FlowEngineDX11
{
	struct TransformBuffer
	{
		DirectX::XMFLOAT4X4 positionMatrix;
		DirectX::XMFLOAT4X4 scaleMatrix;
		DirectX::XMFLOAT4X4 rotationMatrix;
	};
}

