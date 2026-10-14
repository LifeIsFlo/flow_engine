#pragma once
#include "FWD.h"
#include <chrono>

namespace FlowEngineDX11
{
	class Time
	{
	private:
		static std::chrono::time_point<std::chrono::steady_clock> timeLastFrameStart;
		static std::chrono::time_point<std::chrono::steady_clock> timeLastFrameEnd;
	public:
		static float deltaTime;
		static void Init();
		static void BeginFrame();
		static float GetClampedDurationTime();
		static void EndFrame();
	};
}
