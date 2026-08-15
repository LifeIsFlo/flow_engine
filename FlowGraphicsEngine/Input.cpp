#include "Input.h"
#include "Vector2.h"
#include "Keys.h"
#include <vector>
#include <algorithm>

#include "Console.hpp"

LPDIRECTINPUT8 FlowEngineDX11::Input::g_pDirectInput;
LPDIRECTINPUTDEVICE8 FlowEngineDX11::Input::g_pMouseDevice;
LPDIRECTINPUTDEVICE8 FlowEngineDX11::Input::g_pKeyboardDevice;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysPressed;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysDownThisFrame;
std::vector<FlowEngineDX11::Keys> FlowEngineDX11::Input::keysUpThisFrame;
float FlowEngineDX11::Input::mouseDeltaY;
float FlowEngineDX11::Input::mouseDeltaX;
float FlowEngineDX11::Input::mousePosY;
float FlowEngineDX11::Input::mousePosX;
float FlowEngineDX11::Input::mouseScroll;
bool FlowEngineDX11::Input::leftMouseDown = false;
bool FlowEngineDX11::Input::middleMouseDown = false;
bool FlowEngineDX11::Input::rightMouseDown = false;

const std::vector<uint8_t> FlowEngineDX11::Input::KeysToDIK = {
    0,                  // NONE
    DIK_A, DIK_B, DIK_C, DIK_D, DIK_E, DIK_F, DIK_G, DIK_H, DIK_I, DIK_J,
    DIK_K, DIK_L, DIK_M, DIK_N, DIK_O, DIK_P, DIK_Q, DIK_R, DIK_S, DIK_T,
    DIK_U, DIK_V, DIK_W, DIK_X, DIK_Y, DIK_Z,
    DIK_0, DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9, // K0 - K9
    DIK_F1, DIK_F2, DIK_F3, DIK_F4, DIK_F5, DIK_F6,
    DIK_F7, DIK_F8, DIK_F9, DIK_F10, DIK_F11, DIK_F12,
    DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT,
    DIK_SPACE, DIK_TAB, DIK_LSHIFT, DIK_LCONTROL, // DirectInput separates Left/Right modifiers
    DIK_INSERT, DIK_DELETE, DIK_HOME, DIK_END, DIK_PRIOR, DIK_NEXT, // PGUP, PGDN
    DIK_BACK, DIK_ESCAPE, DIK_RETURN, DIK_NUMPADENTER, DIK_PAUSE, DIK_SCROLL,
    DIK_NUMPAD0, DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4,
    DIK_NUMPAD5, DIK_NUMPAD6, DIK_NUMPAD7, DIK_NUMPAD8, DIK_NUMPAD9,
    DIK_MULTIPLY, DIK_DIVIDE, DIK_ADD, DIK_SUBTRACT, DIK_DECIMAL, DIK_PERIOD,
    DIK_EQUALS, DIK_COMMA, DIK_MINUS,
    DIK_COLON, DIK_SLASH, DIK_GRAVE, DIK_LBRACKET,
    DIK_BACKSLASH, DIK_RBRACKET, DIK_APOSTROPHE, 0, // OEM_1 to OEM_8 layout dependent keys
    DIK_CAPITAL // CAPS_LOCK
};


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

    g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, nullptr);
    g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    g_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    g_pKeyboardDevice->Acquire();
}

void FlowEngineDX11::Input::Update() 
{
    if (FAILED(UpdateMouse()))
    {
        g_pMouseDevice->Acquire();
    }

    if (FAILED(UpdateKeyboard()))
    {
        g_pKeyboardDevice->Acquire();
    }
}

HRESULT FlowEngineDX11::Input::UpdateMouse()
{
    DIMOUSESTATE mouseState;
    HRESULT hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
    if (FAILED(hr)) return hr;

    leftMouseDown = mouseState.rgbButtons[0] & 0x80;

    rightMouseDown = mouseState.rgbButtons[1] & 0x80;

    middleMouseDown = mouseState.rgbButtons[2] & 0x80;

    mouseDeltaX = mouseState.lX;
    mouseDeltaY = mouseState.lY;

    mouseScroll = mouseState.lZ;

    return S_OK;
}

HRESULT FlowEngineDX11::Input::UpdateKeyboard()
{
    DIKEYBOARDSTATE keyboardState;
    HRESULT hr = g_pKeyboardDevice->GetDeviceState(sizeof(DIKEYBOARDSTATE), &keyboardState);
    if (FAILED(hr)) return hr;

    for (int i = 0; i < KeysToDIK.size(); i++)
    {
        Keys key = static_cast<Keys>(static_cast<uint8_t>(i));
        if (key != Keys::NONE) {
            if (keyboardState[KeysToDIK[i]] & 0x80)
            {
                keysPressed.push_back(key);
            }
            else
            {
                std::erase_if(keysPressed, [key](const auto& item)
                    {
                        return item == key;
                    });
            }
        }
    }

    return S_OK;
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

FlowEngineDX11::Vector2 FlowEngineDX11::Input::GetMouseDelta()
{
    return {mouseDeltaX, mouseDeltaY};
}

FlowEngineDX11::Vector2 FlowEngineDX11::Input::GetMousePosition()
{
    return {mousePosX, mousePosY};
}

float FlowEngineDX11::Input::GetMouseScroll()
{
    return mouseScroll;
}

void FlowEngineDX11::Input::CleanUp()
{
    if (g_pMouseDevice) {
        g_pMouseDevice->Unacquire();
        g_pMouseDevice->Release();
        g_pMouseDevice = nullptr;
    }
    if (g_pKeyboardDevice)
    {
        g_pKeyboardDevice->Unacquire();
        g_pKeyboardDevice->Release();
        g_pKeyboardDevice = nullptr;
    }
    if (g_pDirectInput) {
        g_pDirectInput->Release();
        g_pDirectInput = nullptr;
    }
}
