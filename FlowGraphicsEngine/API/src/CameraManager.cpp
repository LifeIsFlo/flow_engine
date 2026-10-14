#include "CameraManager.h"
#include "Camera.h"

FlowEngineDX11::Camera* FlowEngineDX11::CameraManager::g_activeCamera = nullptr;

FlowEngineDX11::Camera* FlowEngineDX11::CameraManager::SetActive(Camera* camera)
{
	g_activeCamera = camera;
	return g_activeCamera;
}

FlowEngineDX11::Camera* FlowEngineDX11::CameraManager::GetActive()
{
	return g_activeCamera;
}

bool FlowEngineDX11::CameraManager::isActiveCamera(const Camera* camera)
{
	return g_activeCamera == camera;
}

HRESULT FlowEngineDX11::CameraManager::CreateBuffers(const DX11Protocol* dx11_protocol)
{
	HRESULT hr = g_activeCamera->CreateBuffers(dx11_protocol);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT FlowEngineDX11::CameraManager::UpdateBuffers(const DX11Protocol* dx11_protocol)
{
	HRESULT hr = g_activeCamera->UpdateBuffers(dx11_protocol);
	if (FAILED(hr)) return hr;

	return S_OK;
}

void FlowEngineDX11::CameraManager::ClearState()
{
	g_activeCamera = nullptr;
}
