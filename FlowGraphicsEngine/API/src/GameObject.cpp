#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

namespace FlowEngineDX11 {
	void GameObject::OnInstantiate(FlowEngine* _engine)
	{
		transform = this->AddComponent<Transform>();
	}

	void GameObject::PrepareDestroy()
	{
		for (Component* com : _components)
		{
			com->Destroy();
		}
		_components.clear();
	}

	ObjectRegistry GameObject::GetRegistry()
	{
		return ObjectRegistry::GameObject;
	}
}