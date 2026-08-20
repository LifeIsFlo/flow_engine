#pragma once
#include <iostream>

#include "FWD.h"
#include <string>

namespace FlowEngineDX11 {
	enum class ObjectState : uint8_t { Active, Dead };
	enum class ObjectRegistry : uint8_t {Renderer, Behaviour, Camera, Light, Asset, NONE};
	class Object
	{
	protected:
		FlowEngine* engine = nullptr;
	public:
		std::string name = "newObject";
	private:
		ObjectState m_state = ObjectState::Active;
	public:
		Object();
		virtual ~Object();
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator=(Object&&) = delete;
		bool isDead() const;
		void OnInstantiateObject(FlowEngine* _engine)
		{
			this->engine = _engine;
			this->OnInstantiate(_engine);
		}
		virtual ObjectRegistry GetRegistry() { return ObjectRegistry::NONE; }
		virtual void OnInstantiate(FlowEngine* _engine) {}
		virtual void PrepareDestroy(){}
		virtual void OnDestroy() {}
		static void Destroy(Object* objToDestroy);
		void Destroy();
	};
}
