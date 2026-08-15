#pragma once
#include "FlowEngine.h"
#include "Keys.h"
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
		static LPDIRECTINPUTDEVICE8 g_pKeyboardDevice;
		static std::vector<Keys> keysDownThisFrame;
		static std::vector<Keys> keysPressed;
		static std::vector<Keys> keysUpThisFrame;
		static float mouseScroll;
		static float mouseDeltaX;
		static float mouseDeltaY;
		static float mousePosX;
		static float mousePosY;
		static bool leftMouseDown;
		static bool middleMouseDown;
		static bool rightMouseDown;

		static const std::vector<uint8_t> KeysToDIK;
	public:
		static void Init(HINSTANCE hInstance,HWND hwnd);
		static void RegisterInputDevices(HINSTANCE hInstance, HWND hwnd);
		static void Update();
		static HRESULT UpdateMouse();
		static HRESULT UpdateKeyboard();
		static bool GetKey(Keys key);
		static bool GetKeyDown(Keys key);
		static bool GetKeyUp(Keys key);
		static bool GetMouseDown(int key);
		static Vector2 GetMouseDelta();
		static Vector2 GetMousePosition();
		static float GetMouseScroll();
		static void CleanUp();
	};
}


typedef BYTE DIKEYBOARDSTATE[256];