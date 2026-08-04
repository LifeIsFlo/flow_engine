#pragma once
#include "FWD.h"
#include "Component.h"

namespace FlowEngineDX11
{
	class Behaviour : public Component
	{
	public:
		bool enabled = true;
		virtual void Update() {}
		virtual void Start() {}
		ObjectRegistry GetRegistry() override;
	};
}
