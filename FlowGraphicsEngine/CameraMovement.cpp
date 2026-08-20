#include "CameraMovement.h"

#include <numbers>

#include "Console.hpp"
#include "GameObject.h"
#include "Input.h"
#include "Time.h"
#include "Transform.h"
#include "Vector3.h"
#include "Vector2.h"

void CameraMovement::Update()
{
	if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::R))
	{
		transform->rotation = FlowEngineDX11::Vector3::ZERO;
	}

	if (FlowEngineDX11::Input::GetMouseDown(1)) {
		float x = 0;
		float y = 0;
		float z = 0;

		float speed = .01f;

		FlowEngineDX11::Vector2 mouseDelta = FlowEngineDX11::Input::GetMouseDelta();
		transform->rotation.y += mouseDelta.x / (180 / std::numbers::pi);
		transform->rotation.x += mouseDelta.y / (180 / std::numbers::pi);
		SetCursor(nullptr);


		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::SHIFT))
		{
			speed = 0.02;
		}

		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::A)) {
			x -= speed;
		}
		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::S)) {
			z -= speed;
		}
		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::D)) {
			x += speed;
		}
		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::W))
		{
			z += speed;
		}
		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::E))
		{
			y += speed;
		}
		if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::Q))
		{
			y -= speed;
		}

		transform->position += transform->GetForward() * z;
		transform->position += transform->GetRight() * x;
		transform->position.y += y;
	}
}
