#include "FlowEngine.h"
#include "GameObject.h"
#include <iostream>
#include <windows.h>
#include "FWD.h"
#include "Sprite.h"
#include "CameraMovement.h"
#include "Vector2.h"
#include "PlayerMovement.h"
#include "SpriteRenderer.h"
#include "Transform.h"

int main() {
	FlowEngineDX11::FlowEngine engine(600, 600, false, "FlowEngineDX11");
	FlowEngineDX11::GameObject* gameObj1 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::SpriteRenderer* player_renderer = gameObj1->AddComponent<FlowEngineDX11::SpriteRenderer>();
	gameObj1->AddComponent<PlayerMovement>();

	player_renderer->sprite = new FlowEngineDX11::Sprite(500, 500);

	FlowEngineDX11::GameObject* cameraObj = engine.AddObject<FlowEngineDX11::GameObject>();
	cameraObj->AddComponent<FlowEngineDX11::Camera>();
	cameraObj->AddComponent<CameraMovement>();
	
	if (engine.Run()) {
		std::cout << "Ran successfully!";
	}
	else {
		std::cout << "Didn't run successfully";
	}
	return 0;
}
