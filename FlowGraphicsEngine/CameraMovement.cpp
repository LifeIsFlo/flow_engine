#include "CameraMovement.h"
#include "Input.h"
#include "Time.h"
#include "Transform.h"

void CameraMovement::Update()
{
	if (FlowEngineDX11::Input::GetMouseDown(0))
	{
		transform->position.x += 1;
	}
	else if (FlowEngineDX11::Input::GetMouseDown(1))
	{
		transform->position.x -= 1;
	}
	else if (FlowEngineDX11::Input::GetMouseDown(2))
	{
		transform->rotation += 0.1f;
	}

	//std::cout << "FPS: ";
	//std::cout << std::to_string(1 / FlowEngineDX11::Time::deltaTime) << "\n";
}
