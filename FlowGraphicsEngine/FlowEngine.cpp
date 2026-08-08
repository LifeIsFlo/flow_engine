#include "FlowEngine.h"
#include "Color.h"
#include "Object.h"
#include "FWD.h"
#include "GameObject.h"
#include "DX11Protocol.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include "Camera.h"
#include "Behaviour.h"
#include "Input.h"
#include "Renderer.h"
#include <chrono>

#include "Time.h"


bool FlowEngineDX11::FlowEngine::Tick() {
    Input::Update();

	for (auto behaviour : behaviours)
	{
        behaviour->Update();
	}

    dx11_protocol->Render();

    return true;
}

bool FlowEngineDX11::FlowEngine::AddObjectToRegistry(Object* obj)
{
    switch (obj->GetRegistry()) {
    case ObjectRegistry::Behaviour:
        behaviours.push_back(dynamic_cast<Behaviour*>(obj));
        return true;
    case ObjectRegistry::Renderer:
        dx11_protocol->AddRenderer((dynamic_cast<Renderer*>(obj)));
        return true;
    case ObjectRegistry::Camera:
        dx11_protocol->SetCamera(dynamic_cast<Camera*>(obj));
        return true;
    case ObjectRegistry::NONE:
        return true;
    }
    return false;
}

void FlowEngineDX11::FlowEngine::StartGameLoop()
{
    for (auto behaviour : behaviours)
    {
        behaviour->Start();
    }
    MSG msg = {};
    Time::Init();
    while (true) {
        Time::BeginFrame();
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) break;
        }

        Tick();
        Time::EndFrame();
    }
}

bool FlowEngineDX11::FlowEngine::Run() {
    if (!dx11_protocol->Init()) return false;

    StartGameLoop();

    dx11_protocol->CleanUp();

    return true;
}

FlowEngineDX11::FlowEngine::FlowEngine(const size_t& height, const size_t& width, const bool& fullScreen, const std::string& title) {
    this->dx11_protocol = std::make_unique<DX11Protocol>(height, width, title, fullScreen);
}
