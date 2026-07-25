#pragma once
#include "Engine.h"
#include "ComponentHolderProtocol.h"
#include <iostream>
#include <type_traits>
#include <concepts>

namespace FlowEngine {
	class GameObject : public ComponentHolderProtocol
	{
	public: 
		GameObject();
	public:
		olc::vf2d pos;
		olc::vf2d scale;
	};
}
