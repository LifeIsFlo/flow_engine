#include "Component.h"
#include "GameObject.h"

void FlowEngineDX11::Component::SetGameObject(GameObject* _gameObject)
{
	this->gameObject = _gameObject;
	this->transform = _gameObject->transform;
}
