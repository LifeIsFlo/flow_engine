#include <numbers>

#include "Input.h"
#include "PlayerMovement.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "DX11Protocol.h"
#include "Keys.h"
#include "Transform.h"
#include "Color.h"
#include "Console.hpp"
#include "Time.h"
#include "Vector2.h"
#include "Vector3.h"

void PlayerMovement::Update() 
{
	/*
	if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::A) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::LEFT))) {
		transform->rotation.y -= .01f;
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::S) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::DOWN))) {
		transform->rotation.x -= .01f;
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::D) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::RIGHT))) {
		transform->rotation.y += .01f;
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::W) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::UP)))
	{
		transform->rotation.x += .01f;
	}
	if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::R))
	{
		transform->rotation = FlowEngineDX11::Vector3::ZERO;
	}
	*/

	if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::Z))
	{
		transform->rotation.y += speed * FlowEngineDX11::Time::deltaTime;
	}
}