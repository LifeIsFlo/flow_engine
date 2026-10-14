#pragma once
#include "FWD.h"
#include "Vector3.h"

namespace FlowEngineDX11
{
	struct ViewProjectionBuffer
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		Vector3 position;
		float padding;
	};
}