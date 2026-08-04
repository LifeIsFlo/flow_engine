#pragma once
#include <string>
#include <windows.h>
#include <memory>
#include <chrono>
#include <vector>
#include <cstdint>

namespace FlowEngineDX11
{
	class Scene;
	class Object;
	enum class ObjectState : uint8_t;
	enum class ObjectRegistry : uint8_t;
	class GameObject;
	class Behaviour;
	class FlowEngine;
	class Component;
	class Renderer;
	class Transform;
	struct Vector2;
	struct Vertex2D;
	class Input;
	class SceneManager;
	enum class Keys : uint8_t;
	struct Color;
}
