#pragma once
#include "Engine.h"
#include "Component.h"

namespace FlowEngine {
	class Renderer : public Component
	{
	protected:
		Renderer(){}
	public:
		virtual void Render(Engine* engine) {}
	};
}