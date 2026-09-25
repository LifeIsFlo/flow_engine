#pragma once
#include "FWD.h"
#include "Component.h"

namespace FlowEngineDX11
{
	class Behaviour : public Component
	{
		bool m_enabled = true;
	public:
		virtual void Update() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void SetEnabled(bool enabled);
		ObjectRegistry GetRegistry() override;
	};
}
