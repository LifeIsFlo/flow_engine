#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

void FlowEngineDX11::GameObject::OnInstantiate(FlowEngine* _engine)
{
	transform = this->AddComponent<Transform>();
}

void FlowEngineDX11::GameObject::PrepareDestroy()
{
	for (Component* com : _components)
	{
		com->Destroy();
	}
}
