#pragma once
#include <string>
#include <windows.h>
#include <memory>
#include <chrono>
#include <vector>
#include "FWD.h"
#include "Object.h"
#include "DX11Protocol.h"

namespace FlowEngineDX11
{
	class FlowEngine
	{
	private:
		std::unique_ptr<DX11Protocol> dx11_protocol;
		std::vector<std::unique_ptr<Object>> objects;
		std::vector<Behaviour*> behaviours;
	public:
		FlowEngine(const size_t& height, const size_t& width, const bool& fullScreen, const std::string& title);
		void StartGameLoop();
		bool Run();
		bool Tick();
		bool AddObjectToRegistry(Object* obj);
	public:
		template<typename T>
		requires std::derived_from<T, Object>
		T* AddObject()
		{
			std::unique_ptr<T> objPtr = std::make_unique<T>();
			T* pObj = objPtr.get();
			if (AddObjectToRegistry(pObj)) {
				pObj->OnInstantiateObject(this);
				objects.push_back(std::move(objPtr));
				return pObj;
			}
			std::cout << "Couldn't create Object";
			return nullptr;
		}
	};
}
