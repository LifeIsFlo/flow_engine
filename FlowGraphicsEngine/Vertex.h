#pragma once
#include "FWD.h"
#include "Vector3.h"
#include "Color.h"

namespace FlowEngineDX11
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		Color color;
	};
}
