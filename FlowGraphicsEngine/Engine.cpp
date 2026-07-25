#include "Engine.h"
#include "Renderer.h"
#include "Object.h"
#include <memory>


FlowEngine::Engine* FlowEngine::Engine::current = nullptr;

bool FlowEngine::Engine::OnUserUpdate(float fElapsedTime) {
    Clear(olc::Pixel(0, 0, 0, 255));

    for (const auto& objPtr : updatables) {
        if (!objPtr) continue; // Safety check

        // 1. Call the virtual Update function
        objPtr->Update(fElapsedTime);

        // 2. Safely check if it can be cast to a Renderer using pointer syntax
        if (Renderer* renderer = dynamic_cast<Renderer*>(objPtr.get())) {
            renderer->Render(this); // Successfully found a Renderer!
        }
        // No catch block needed! If it's not a Renderer, it safely returns nullptr
    }
    return true;
}



template<typename T>
requires std::derived_from<T, FlowEngine::Object>
T* FlowEngine::Engine::Add_Object(T& obj) {
    return current->AddObject(obj);
}

template<typename T>
requires std::derived_from<T, FlowEngine::Object>
T* FlowEngine::Engine::AddObject(T& obj) {
    std::unique_ptr<T> pObj = std::make_unique<T>(obj);
    T* pRet = pObj.get();

    if (IUpdatable* updatable = dynamic_cast<IUpdatable*>(pRet)) {
        updatables.push_back(updatable);
    }

    objects.push_back(std::move(pObj));
    return pRet;
}

FlowEngine::Engine::~Engine() {
	// Clean up the tracker when this instance dies
	if (current == this) {
		current = nullptr;
	}
}

FlowEngine::Engine::Engine() {
	// Set the global tracker to this instance
	current = this;
	sAppName = "yo momma";
}

bool FlowEngine::Engine::OnUserCreate() {
	return true;
}

olc::HWButton FlowEngine::Engine::Get_Key(olc::Key key) {
	return current->GetKey(key);
}