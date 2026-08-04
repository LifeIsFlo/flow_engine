#include "PlayerRenderer.h"
#include "Input.h"
#include "PlayerMovement.h"
#include "DX11Protocol.h"
#include "Key.h"
#include "Transform.h"
#include "Color.h"
#include "Vector2.h"

void PlayerMovement::Update() {
	if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::A) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::LEFT))) {
		transform->pos += FlowEngineDX11::Vector2(-speed, 0);
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::S) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::DOWN))) {
		transform->pos += FlowEngineDX11::Vector2(0, speed);
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::D) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::RIGHT))) {
		transform->pos += FlowEngineDX11::Vector2(speed, 0);
	}
	else if ((FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::W) && wasd) || (!wasd && FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::UP)))
	{
		transform->pos += FlowEngineDX11::Vector2(0, -speed);
	}
	else if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::V))
	{
		speed++;
	}
	else if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::C))
	{
		speed--;
	}
	else if (FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::N))
	{
		transform->scale.x -= 1;
		transform->scale.y -= 1;
	}
	else if(FlowEngineDX11::Input::GetKey(FlowEngineDX11::Keys::M))
	{
		transform->scale.x += 1;
		transform->scale.y += 1;
	}
}


void PlayerRenderer::OnRender(ID3D11DeviceContext* g_context, ID3D11Device* g_device)
{
	
}
