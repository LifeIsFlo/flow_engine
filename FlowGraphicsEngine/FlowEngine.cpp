#include "FlowEngine.h"
#include "Object.h"
#include "GameObject.h"
#include "DX11Protocol.h"
#include <string>
#include "Camera.h"
#include "Behaviour.h"
#include "Light.h"
#include "Input.h"
#include "Renderer.h"
#include "LightingProtocol.h"
#include "RenderProtocol.h"
#include "Console.hpp"
#include "BehaviourProtocol.h"
#include "CameraManager.h"
#include "AssetDatabase.h"
#include "Time.h"

bool FlowEngineDX11::FlowEngine::Tick() 
{
    Input::Update();

    behaviourProtocol->Update();

    // Rendering
    renderProtocol->UpdateBuffers(dx11_protocol.get());

    // Shadow pass
    dx11_protocol->PrepareShadowPass();

    lightingProtocol->ShadowPass(this, dx11_protocol.get());


    // Main pass
    dx11_protocol->PrepareMainPass();

    CameraManager::UpdateBuffers(dx11_protocol.get());

    lightingProtocol->MainPass(dx11_protocol.get());

    renderProtocol->Render(dx11_protocol.get());

    dx11_protocol->EndFrame();

    // Object resolving
    ResolveDestroyedObjects();

    return true;
}

bool FlowEngineDX11::FlowEngine::AddObjectToRegistry(Object* obj)
{
    switch (obj->GetRegistry()) {
    case ObjectRegistry::Behaviour:
        behaviourProtocol->AddBehaviour(dynamic_cast<Behaviour*>(obj));
        return true;
    case ObjectRegistry::Renderer: {
        auto renderer = dynamic_cast<Renderer*>(obj);
        if (renderer->castShadows) lightingProtocol->AddShadow(renderer);
        renderProtocol->AddRenderer(renderer);
        return true;
    }
    case ObjectRegistry::Camera:
        CameraManager::SetActive(dynamic_cast<Camera*>(obj));
        return true;
    case ObjectRegistry::Light:
        lightingProtocol->AddLight(dynamic_cast<Light*>(obj));
        return true;
    case ObjectRegistry::GameObject:
        gameObjects.push_back(dynamic_cast<GameObject*>(obj));
        return true;
    default:
        return true;
    }
}

void FlowEngineDX11::FlowEngine::RemoveObjectFromRegistry(Object* obj)
{
	switch (obj->GetRegistry())
	{
	case ObjectRegistry::Behaviour:
        behaviourProtocol->RemoveBehaviour(dynamic_cast<Behaviour*>(obj));
        break;
    case ObjectRegistry::Renderer: {
        auto renderer = dynamic_cast<Renderer*>(obj);
        lightingProtocol->RemoveShadow(renderer);
        renderProtocol->RemoveRenderer(renderer);
        break;
    }
	case ObjectRegistry::Light:
        lightingProtocol->RemoveLight(dynamic_cast<Light*>(obj));
        break;
    case ObjectRegistry::GameObject:
        std::erase_if(gameObjects, [obj](const GameObject* go) 
            {
                return obj == go;
            });
        break;
	case ObjectRegistry::Camera:
        Camera* camera = dynamic_cast<Camera*>(obj);
        if (CameraManager::isActiveCamera(camera)) CameraManager::SetActive(nullptr);
        break;
	}
}

void FlowEngineDX11::FlowEngine::ResolveDestroyedObjects()
{
    int deadObjects = 0;

	for (auto& obj : objects)
	{
        if (obj->isDead())
        {
            RemoveObjectFromRegistry(obj.get());
            deadObjects++;
        }
	}

    std::erase_if(objects, [](const std::unique_ptr<Object>& element)
    {
        return element->isDead();
    });

    if (deadObjects > 0) {
        objects.shrink_to_fit();
    }
}

void FlowEngineDX11::FlowEngine::StartGameLoop()
{
    MSG msg = {};
    running = true;
    while (running) 
    {
        Time::BeginFrame();
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) running = false;
        }
        if (Input::GetKey(Keys::ESCAPE) || requestExit) running = false;
        Tick();
        Time::EndFrame();
    }
}

void FlowEngineDX11::FlowEngine::CleanUp()
{
    ClearState();
    CameraManager::ClearState();
    Input::CleanUp();
    AssetDatabase::CleanUp();
    dx11_protocol->CleanUp();
}

void FlowEngineDX11::FlowEngine::ClearState()
{
    for (auto go : gameObjects)
    {
        go->Destroy();
    }
    ResolveDestroyedObjects();
    objects.clear();
    objects.shrink_to_fit();
    gameObjects.clear();
    gameObjects.shrink_to_fit();
    flowio::Console::WriteLine("objects in vector: " + std::to_string(objects.size()));
    objectsDestroyed = true;

    lightingProtocol->ClearState();
    renderProtocol->ClearState();
    behaviourProtocol->ClearState();
    dx11_protocol->ClearState();
    CameraManager::ClearState();
    AssetDatabase::CleanUp();
}

void FlowEngineDX11::FlowEngine::RequestExit()
{
    requestExit = true;
}

HRESULT FlowEngineDX11::FlowEngine::CreateBuffers()
{
    behaviourProtocol->Start();

    return S_OK;
}

HRESULT FlowEngineDX11::FlowEngine::CreateResources()
{
    HRESULT hr = dx11_protocol->CreateResources();
    if (FAILED(hr)) return hr;
    hr = CameraManager::CreateBuffers(dx11_protocol.get());
    if (FAILED(hr)) return hr;
    hr = renderProtocol->CreateResources(dx11_protocol.get());
    if (FAILED(hr)) return hr;
    hr = lightingProtocol->CreateResources(dx11_protocol.get());
    if (FAILED(hr)) return hr;
    hr = AssetDatabase::CreateBuffers(dx11_protocol.get());
    if (FAILED(hr)) return hr;
    return S_OK;
}

HRESULT FlowEngineDX11::FlowEngine::Run() 
{
    Camera* activeCam = CameraManager::GetActive();

    if (!activeCam) return E_FAIL;

    Time::Init();

    HRESULT hr = dx11_protocol->Init(&hwnd, &hInstance);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Failed to initialize DirectX 11", L"Error", MB_OK);
        CleanUp();
        return hr;
    }

    hr = CreateResources();
    if (FAILED(hr))
    {
        MessageBox(hwnd, L"Couldn't make the proper resources", L"Error", MB_OK);
        CleanUp();
        return hr;
    }

    Input::Init(hInstance, hwnd);

    StartGameLoop();

    CleanUp();

    return S_OK;
}

FlowEngineDX11::FlowEngine::FlowEngine(const size_t& height, const size_t& width, const std::string& title, const bool& vSync) {
    this->dx11_protocol = std::make_unique<DX11Protocol>(height, width, title, vSync);
    this->behaviourProtocol = std::make_unique<BehaviourProtocol>();
    this->renderProtocol = std::make_unique<RenderProtocol>();
    this->lightingProtocol = std::make_unique<LightingProtocol>();
}
