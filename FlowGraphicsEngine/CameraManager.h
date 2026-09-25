#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {
	class CameraManager
	{
		static std::vector<Camera*> cameras;
		static Camera* g_activeCamera;
	public:
		static Camera* SetActive(Camera* camera);
		static Camera* GetActive();
		static bool isActiveCamera(const Camera* camera);

		static HRESULT CreateBuffers(const DX11Protocol* dx11_protocol);
		static HRESULT UpdateBuffers(const DX11Protocol* dx11_protocol);

		static void ClearState();
	};
}
