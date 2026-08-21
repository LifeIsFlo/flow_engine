#include "FlowEngine.h"
#include "GameObject.h"
#include <iostream>
#include <windows.h>

#include "AssetDatabase.h"
#include "FWD.h"
#include "Sprite.h"
#include "CameraMovement.h"
#include "Console.hpp"
#include "Mesh.h"
#include "Vector3.h"
#include "PlayerMovement.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "Transform.h"

int main() 
{
	FlowEngineDX11::FlowEngine engine(1080, 1920, true, "FlowEngineDX11", true);

	FlowEngineDX11::Mesh* street_lamp_mesh = FlowEngineDX11::AssetDatabase::LoadModel("street lamp.obj")->GetMesh("Lamp");
	FlowEngineDX11::Mesh* cube_mesh = FlowEngineDX11::AssetDatabase::LoadModel("Cube.obj")->GetMesh("Cube");

	// Player 1
	FlowEngineDX11::GameObject* gameObj1 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* player_renderer = gameObj1->AddComponent<FlowEngineDX11::MeshRenderer>();
	gameObj1->AddComponent<PlayerMovement>();
	player_renderer->mesh = street_lamp_mesh;
	
	// Player 2
	FlowEngineDX11::GameObject* gameObj2 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* player_renderer2 = gameObj2->AddComponent<FlowEngineDX11::MeshRenderer>();
	player_renderer2->mesh = cube_mesh;
	player_renderer2->sprite = FlowEngineDX11::AssetDatabase::LoadSprite("test");

	gameObj2->transform->position.y = -0.1f;
	gameObj2->transform->scale = FlowEngineDX11::Vector3(10, 0.1f, 10);
	// Camera
	FlowEngineDX11::GameObject* cameraObj = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::Camera* cam = cameraObj->AddComponent<FlowEngineDX11::Camera>();
	cameraObj->AddComponent<CameraMovement>();
	cameraObj->transform->position.z -= 10;
	cameraObj->transform->position.y += 2;
	cameraObj->transform->rotation.x += 0.2f;
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
