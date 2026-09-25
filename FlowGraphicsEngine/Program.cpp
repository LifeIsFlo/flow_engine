#include "FlowEngine.h"
#include "FWD.h"
#include "GameObject.h"
#include "AssetDatabase.h"
#include "Sprite.h"
#include "CameraMovement.h"
#include "Mesh.h"
#include "Light.h"
#include "Color.h"
#include "Vector3.h"
#include "PlayerMovement.h"
#include "RenderProtocol.h"
#include "BehaviourProtocol.h"
#include "SpriteRenderer.h"
#include "VertexShader.h"
#include "Camera.h"
#include "PixelShader.h"
#include "LightingProtocol.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "Transform.h"
#include "ShadowMap.h"

static void run() 
{
	FlowEngineDX11::FlowEngine engine(1080, 1920, "FlowEngineDX11", true);

	FlowEngineDX11::Mesh* street_lamp_mesh = FlowEngineDX11::AssetDatabase::LoadModel("street lamp.obj")->GetMesh("Lamp");
	FlowEngineDX11::Mesh* glass_mesh = FlowEngineDX11::AssetDatabase::LoadModel("street lamp.obj")->GetMesh("Glass");
	FlowEngineDX11::Mesh* cube_mesh = FlowEngineDX11::AssetDatabase::LoadModel("Cube.obj")->GetMesh("Cube");
	FlowEngineDX11::Sprite* sprite = FlowEngineDX11::AssetDatabase::LoadSprite("cat.png");

	// Street lamp
	FlowEngineDX11::GameObject* gameObj1 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* player_renderer = gameObj1->AddComponent<FlowEngineDX11::MeshRenderer>();
	//FlowEngineDX11::MeshRenderer* glass_renderer = gameObj1->AddComponent<FlowEngineDX11::MeshRenderer>();
	player_renderer->mesh = street_lamp_mesh;
	//glass_renderer->mesh = glass_mesh;

	// Ground
	FlowEngineDX11::GameObject* gameObj2 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* player_renderer2 = gameObj2->AddComponent<FlowEngineDX11::MeshRenderer>();
	player_renderer2->mesh = cube_mesh;
	//player_renderer2->sprite = sprite;
	gameObj2->transform->position.y = -0.1f;
	gameObj2->transform->scale = FlowEngineDX11::Vector3(10, 0.1f, 10);

	// Cube
	FlowEngineDX11::GameObject* cube = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* cube_renderer = cube->AddComponent<FlowEngineDX11::MeshRenderer>();
	cube->AddComponent<PlayerMovement>();
	cube_renderer->mesh = cube_mesh;
	cube_renderer->sprite = sprite;
	cube->transform->position = FlowEngineDX11::Vector3(1.5f, 3.20f, 0.0f);
	cube->transform->scale = FlowEngineDX11::Vector3(0.5f, 0.5f, 0.5f);

	// Cube 2
	FlowEngineDX11::GameObject* cube2 = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::MeshRenderer* cube_renderer2 = cube2->AddComponent<FlowEngineDX11::MeshRenderer>();
	cube_renderer2->mesh = cube_mesh;
	cube_renderer->sprite = sprite;
	cube2->transform->position = FlowEngineDX11::Vector3(3.0f, 3.20f, 0.0f);
	cube2->transform->scale = FlowEngineDX11::Vector3(0.1f, 5.0f, 5.0f);

	// Light
	FlowEngineDX11::GameObject* lightObj = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::Light* light = lightObj->AddComponent<FlowEngineDX11::Light>();
	lightObj->transform->position = { 0, 3.20f, 0 };
	light->color = FlowEngineDX11::Color(0, 0, 255);
	light->radius = 0;
	light->intensity = 5.0f;

	// Camera
	FlowEngineDX11::GameObject* cameraObj = engine.AddObject<FlowEngineDX11::GameObject>();
	FlowEngineDX11::Camera* cam = cameraObj->AddComponent<FlowEngineDX11::Camera>();
	cameraObj->AddComponent<CameraMovement>();
	//auto camLight = cameraObj->AddComponent<FlowEngineDX11::Light>();
	cameraObj->transform->position.z -= 10;
	cameraObj->transform->position.y += 2;
	cameraObj->transform->rotation.x += 0.2f;
	cam->backgroundColor = FlowEngineDX11::Color(100, 100, 100);
	cam->projectionMode = FlowEngineDX11::Camera::Perspective;

	HRESULT hr = engine.Run();

	// Engine start
	if (SUCCEEDED(hr)) {
		std::cout << "Ran successfully!";
	}
	else {
		std::cout << "Didn't run successfully";
	}
}

int main() 
{
	run();
	return 0;
}
