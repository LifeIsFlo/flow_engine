#pragma once
#include "Console.hpp"
#include <string>
#include <windows.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <numbers>
#include <cstdint>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <span>
#include <array>
#include <map>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace FlowEngineDX11
{
	class Scene;
	class Object;
	enum class ObjectState : uint8_t;
	enum class ObjectRegistry : uint8_t;
	class Time;
	class Camera;
	class GameObject;
	class Behaviour;
	class FlowEngine;
	class Component;
	class Light;
	struct LightData;
	struct ShadowMap;
	struct ViewProjectionBuffer;
	struct TransformBuffer;
	class Renderer;
	class Material;
	class AssetDatabase;
	struct AssetRegistry;
	class ObjLoader;
	class SpriteLoader;
	class SpriteRenderer;
	class MeshRenderer;
	class Mesh;
	class Model;
	class Sprite;
	class Transform;
	struct Vector2;
	struct Vector3;
	struct Vertex;
	class Input;
	class SceneManager;
	enum class Keys : uint8_t;
	class VertexShader;
	class PixelShader;
	struct Color;
	struct GPUColor;
	class BehaviourProtocol;
	class CameraManager;
	class RenderProtocol;
	class LightingProtocol;
	class DX11Protocol;
}
