#pragma once
#include "FWD.h"
#include "Object.h"

namespace FlowEngineDX11 {
	class Component : public Object
	{
	public:
		GameObject* gameObject = nullptr;
		Transform* transform = nullptr;
		void SetGameObject(GameObject* _gameObject);
	};
}
