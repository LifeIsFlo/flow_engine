#include "Input.h"
#include "FWD.h"
#include "Key.h"
#include <vector>
#include <algorithm>

LPDIRECTINPUT8 FlowEngineDX11::Input::g_pDirectInput;
LPDIRECTINPUTDEVICE8 FlowEngineDX11::Input::g_pMouseDevice;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysPressed;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysDownThisFrame;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysUpThisFrame;
bool FlowEngineDX11::Input::leftMouseDown = false;
bool FlowEngineDX11::Input::middleMouseDown = false;
bool FlowEngineDX11::Input::rightMouseDown = false;


void FlowEngineDX11::Input::Init(HINSTANCE hInstance, HWND hwnd)
{
	keysPressed.push_back(Keys::NONE);
	keysDownThisFrame.push_back(Keys::NONE);
	keysUpThisFrame.push_back(Keys::NONE);
    RegisterInputDevices(hInstance, hwnd);
}

void FlowEngineDX11::Input::RegisterInputDevices(HINSTANCE hInstance, HWND hwnd) {
    DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDirectInput, nullptr);

    // Create Mouse Device
    g_pDirectInput->CreateDevice(GUID_SysMouse, &g_pMouseDevice, nullptr);
    g_pMouseDevice->SetDataFormat(&c_dfDIMouse);
    g_pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    g_pMouseDevice->Acquire();
}

void FlowEngineDX11::Input::Update() 
{

    DIMOUSESTATE mouseState;
    HRESULT hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);


    if (FAILED(hr)) {
        g_pMouseDevice->Acquire();
        return;
    }

    if (mouseState.rgbButtons[0] & 0x80) {
        leftMouseDown = true;
    }
    else
    {
        leftMouseDown = false;
    }

    if (mouseState.rgbButtons[1] & 0x80)
    {
        rightMouseDown = true;
    }
    else
    {
        rightMouseDown = false;
    }

    if (mouseState.rgbButtons[2] & 0x80)
    {
        middleMouseDown = true;
    }
    else
    {
        middleMouseDown = false;
    }
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

bool FlowEngineDX11::Input::GetMouseDown(int key)
{
    switch (key)
    {
    case 0:
        return leftMouseDown;
    case 1:
        return rightMouseDown;
    case 2:
        return middleMouseDown;
    default:
        return false;
    }
}

void FlowEngineDX11::Input::CleanUp()
{
    if (g_pMouseDevice) {
        g_pMouseDevice->Unacquire();
        g_pMouseDevice->Release();
        g_pMouseDevice = nullptr;
    }
    if (g_pDirectInput) {
        g_pDirectInput->Release();
        g_pDirectInput = nullptr;
    }
}
