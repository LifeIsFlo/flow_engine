#pragma once
#include "Engine.h"
#include "Object.h"
#include "IUpdatable.h"

namespace FlowEngine {
	class Component : public Object, public IUpdatable
	{
	public:
		GameObject* _gameObject;
	};
}
