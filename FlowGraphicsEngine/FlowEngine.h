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
		std::unique_ptr<BehaviourProtocol> behaviourProtocol;
		std::unique_ptr<RenderProtocol> renderProtocol;
		std::unique_ptr<LightingProtocol> lightingProtocol;
		std::unique_ptr<DX11Protocol> dx11_protocol;
		std::vector<std::unique_ptr<Object>> objects;
		std::vector<GameObject*> gameObjects;

		HWND hwnd = nullptr;
		HINSTANCE hInstance = nullptr;
		bool running = false;
		bool requestExit = false;
		bool objectsDestroyed = true;
	public:
		FlowEngine(const size_t& height, const size_t& width, const std::string& title, const bool& vSync);
		void StartGameLoop();
		void CleanUp();
		void ClearState();
		void RequestExit();

		HRESULT CreateBuffers();
		HRESULT CreateResources();

		HRESULT Run();
		bool Tick();
		bool AddObjectToRegistry(Object* obj);
		void RemoveObjectFromRegistry(Object* obj);
		void ResolveDestroyedObjects();
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
