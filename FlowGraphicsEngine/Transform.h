#pragma once
#include "FWD.h"
#include "Vector2.h"
#include "Object.h"
#include "Component.h"

namespace FlowEngineDX11 {
	class Transform : public Component {
	public:
		Vector2 scale{1,1};
		Vector2 pos{0,0};
		float rotation = 0;
	};
}