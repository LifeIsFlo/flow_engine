#include "FWD.h"
#include "Object.h"

FlowEngineDX11::Object::Object() = default;
FlowEngineDX11::Object::~Object()
{
	std::cout << name + " released\n";
};

void FlowEngineDX11::Object::Destroy(Object* objToDestroy) {
	objToDestroy->Destroy();
}

void FlowEngineDX11::Object::Destroy() {
	PrepareDestroy();
	m_state = ObjectState::Dead;
}

bool FlowEngineDX11::Object::isDead() const {
	return m_state == ObjectState::Dead;
}