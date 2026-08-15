#include "FlowEngine.h"
#include "GameObject.h"
#include <iostream>
#include <windows.h>

#include "AssetDatabase.h"
#include "FWD.h"
#include "Sprite.h"
#include "CameraMovement.h"
#include "Mesh.h"
#include "Vector2.h"
#include "PlayerMovement.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "Transform.h"

int main() {
	FlowEngineDX11::FlowEngine engine(1080, 1920, true, "FlowEngineDX11");

	FlowEngineDX11::Mesh* cubeMesh = FlowEngineDX11::AssetDatabase::LoadMesh("Cube.obj");

	// Player 1
	FlowEngineDX11::GameObject* gameObj1 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* player_renderer = gameObj1->AddComponent<FlowEngineDX11::MeshRenderer>();
	gameObj1->AddComponent<PlayerMovement>();

	player_renderer->mesh = cubeMesh;


	// Player 2
	FlowEngineDX11::GameObject* gameObj2 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* mesh_renderer = gameObj2->AddComponent<FlowEngineDX11::MeshRenderer>();
	gameObj2->AddComponent<PlayerMovement>();


	mesh_renderer->mesh = cubeMesh;

	gameObj2->transform->position.x = 2.1f;
	gameObj2->transform->position.y = .2f;


	// Camera
	FlowEngineDX11::GameObject* cameraObj = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::Camera* cam = cameraObj->AddComponent<FlowEngineDX11::Camera>();
	cameraObj->AddComponent<CameraMovement>();
	cameraObj->transform->position.z -= 2;
	cam->projectionMode = FlowEngineDX11::Camera::Perspective;
	

	// Engine start
	if (engine.Run()) {
		std::cout << "Ran successfully!";
	}
	else {
		std::cout << "Didn't run successfully";
	}
	return 0;
}
