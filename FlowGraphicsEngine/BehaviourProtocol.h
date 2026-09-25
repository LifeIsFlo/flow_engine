#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {
	class BehaviourProtocol
	{
		std::vector<Behaviour*> behaviours;

	public:
		void Start();
		void Update();
		Behaviour* AddBehaviour(Behaviour* behaviour);
		void RemoveBehaviour(Behaviour* behaviour);
		void ClearState();
	};
}
