#include "BehaviourProtocol.h"
#include "Behaviour.h"

void FlowEngineDX11::BehaviourProtocol::Start()
{
    for (auto behaviour : behaviours)
    {
        behaviour->Start();
    }
}

void FlowEngineDX11::BehaviourProtocol::Update()
{
    for (auto behaviour : behaviours)
    {
        behaviour->Update();
    }
}

FlowEngineDX11::Behaviour* FlowEngineDX11::BehaviourProtocol::AddBehaviour(Behaviour* behaviour)
{
    behaviours.push_back(behaviour);
    return behaviour;
}

void FlowEngineDX11::BehaviourProtocol::RemoveBehaviour(Behaviour* behaviour)
{
    std::erase_if(behaviours, [behaviour](const Behaviour* element)
        {
            return element == behaviour;
        });
}

void FlowEngineDX11::BehaviourProtocol::ClearState()
{
    behaviours.clear();
    behaviours.shrink_to_fit();
}
