#pragma once
#include <iostream>

#include "FWD.h"
#include <string>

namespace FlowEngineDX11 {
	enum class ObjectState : uint8_t { Active, Dead };
	enum class ObjectRegistry : uint8_t {Renderer, Behaviour, NONE};
	class Object
	{
	public:
		bool isDead() const;
	protected:
		FlowEngine* engine = nullptr;
	private:
		ObjectState m_state = ObjectState::Active;
	public:
		Object();
		virtual ~Object();
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
