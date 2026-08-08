#include "Time.h"
#include <chrono>
#include <algorithm>
#include <Windows.h>

std::chrono::time_point<std::chrono::steady_clock> FlowEngineDX11::Time::timeLastFrameStart = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> FlowEngineDX11::Time::timeLastFrameEnd = std::chrono::high_resolution_clock::now();
float FlowEngineDX11::Time::deltaTime = 0.1f;

void FlowEngineDX11::Time::Init()
{
	timeLastFrameEnd = std::chrono::high_resolution_clock::now();
}

void FlowEngineDX11::Time::BeginFrame()
{
	deltaTime = GetClampedDurationTime();
	timeLastFrameStart = std::chrono::high_resolution_clock::now();
}

float FlowEngineDX11::Time::GetClampedDurationTime()
{
	return min(std::chrono::duration<float>(timeLastFrameEnd - timeLastFrameStart).count(), 0.1f);
}

void FlowEngineDX11::Time::EndFrame()
{
	timeLastFrameEnd = std::chrono::high_resolution_clock::now();
}