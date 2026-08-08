#pragma once
#include "FlowEngine.h"
#include "Key.h"
#include <vector>
#include <d3d11.h>
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

namespace FlowEngineDX11 {
	class Input
	{
	private:
		static LPDIRECTINPUT8       g_pDirectInput;
		static LPDIRECTINPUTDEVICE8 g_pMouseDevice;
		static std::vector<Keys> keysDownThisFrame;
		static std::vector<Keys> keysPressed;
		static std::vector<Keys> keysUpThisFrame;
		static bool leftMouseDown;
		static bool middleMouseDown;
		static bool rightMouseDown;
	public:
		static void Init(HINSTANCE hInstance,HWND hwnd);
		static void RegisterInputDevices(HINSTANCE hInstance, HWND hwnd);
		static void Update();
		static bool GetKey(Keys key);
		static bool GetKeyDown(Keys key);
		static bool GetKeyUp(Keys key);
		static bool GetMouseDown(int key);
		static void CleanUp();
	};
}
