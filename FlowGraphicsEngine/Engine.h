#pragma once
#include "olcPixelGameEngine.h"
#include <memory>
#include <vector>
#include <functional>

namespace FlowEngine 
{
	class Object;
	class IUpdatable;
	class Component;
	class Renderer;
	class Transform;
	class GameObject;
	class ComponentHolderProtocol;

	class Engine : public olc::PixelGameEngine
	{
	public:
		static Engine* current;
		std::vector<std::unique_ptr<Object>> objects;
		std::vector<std::unique_ptr<IUpdatable>> updatables;

		Engine();
		~Engine();

	public:
		static olc::HWButton Get_Key(olc::Key key);
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

		template<typename T>
		requires std::derived_from<T, Object>
		T* AddObject(T& obj);

		template<typename T>
		requires std::derived_from<T, Object>
		static T* Add_Object(T& obj);
	};
}