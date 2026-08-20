#pragma once
#include "Component.h"
#include "Color.h"
#include "FWD.h"

namespace FlowEngineDX11
{
	class Light : public Component
	{
	public:
		float radius;
		float intensity;
		Color color;

		ObjectRegistry GetRegistry() override;
	};
}
