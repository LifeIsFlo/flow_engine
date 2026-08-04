#pragma once
#include "FlowEngine.h"
#include "Key.h"
#include <vector>

namespace FlowEngineDX11 {
	class Input
	{
		static std::vector<Keys> keysDownThisFrame;
		static std::vector<Keys> keysPressed;
		static std::vector<Keys> keysUpThisFrame;
	public:
		static void Init();
		static void Update(FlowEngine* engine);
		static bool GetKey(Keys key);
		static bool GetKeyDown(Keys key);
		static bool GetKeyUp(Keys key);
	};
}
