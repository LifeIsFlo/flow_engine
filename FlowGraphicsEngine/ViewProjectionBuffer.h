#pragma once
#include <DirectXMath.h>

#include "FWD.h"

namespace FlowEngineDX11
{
	struct ViewProjectionBuffer
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};
}