#include "ComponentHolderProtocol.h"
#include "Component.h"
#include "GameObject.h"

template<typename T>
requires std::derived_from<T, FlowEngine::Component>
T* FlowEngine::ComponentHolderProtocol::AddComponent() {
    auto com = std::make_unique<T>();
    T* pCom = com.get();

    pCom->_gameObject = this;

    _components.push_back(std::move(com));
    return pCom;
}