#include "Behaviour.h"

void FlowEngineDX11::Behaviour::SetEnabled(bool enabled)
{
	if (enabled && !m_enabled) 
	{
		OnEnable();
	}
	else if (m_enabled) {
		OnDisable();
	}

	m_enabled = enabled;
}

FlowEngineDX11::ObjectRegistry FlowEngineDX11::Behaviour::GetRegistry()
{
	return ObjectRegistry::Behaviour;
}
