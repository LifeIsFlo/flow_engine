#include "Input.h"
#include "FWD.h"
#include "Key.h"
#include <vector>
#include <algorithm>

std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysPressed;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysDownThisFrame;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysUpThisFrame;

void FlowEngineDX11::Input::Init()
{
	keysPressed.push_back(Keys::NONE);
	keysDownThisFrame.push_back(Keys::NONE);
	keysUpThisFrame.push_back(Keys::NONE);
}

void FlowEngineDX11::Input::Update(FlowEngine* engine) {

}

bool FlowEngineDX11::Input::GetKey(const Keys key) {
	return std::ranges::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end();
}

bool FlowEngineDX11::Input::GetKeyDown(const Keys key)
{
	return std::ranges::find(keysDownThisFrame.begin(), keysDownThisFrame.end(), key) != keysDownThisFrame.end();
}

bool FlowEngineDX11::Input::GetKeyUp(const Keys key)
{
	return std::ranges::find(keysUpThisFrame.begin(), keysUpThisFrame.end(), key) != keysUpThisFrame.end();
}
