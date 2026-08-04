#include "FlowEngine.h"
#include "GameObject.h"
#include <iostream>
#include <windows.h>
#include "Vector2.h"
#include "PlayerMovement.h"
#include "PlayerRenderer.h"
#include "Transform.h"

int main() {
	FlowEngineDX11::FlowEngine engine(600, 800, false, "FlowEngineDX11");
	FlowEngineDX11::GameObject* gameObj1 = engine.AddObject<FlowEngineDX11::GameObject>();
	gameObj1->transform->scale = FlowEngineDX11::Vector2(50, 50);
	gameObj1->AddComponent<PlayerRenderer>();
	gameObj1->AddComponent<PlayerMovement>();
	
	if (engine.Run()) {
		std::cout << "Ran successfully!";
	}
	else {
		std::cout << "Didn't run successfully";
	}
	return 0;
}
