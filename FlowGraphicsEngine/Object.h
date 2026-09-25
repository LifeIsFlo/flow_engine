#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {
	enum class ObjectState : uint8_t { Active, Dead };
	enum class ObjectRegistry : uint8_t {Renderer = 0, Behaviour, Camera, Light, Asset, GameObject, NONE};
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
		// Called when the object is instantiated into a FlowEngine
		virtual void OnInstantiate(FlowEngine* _engine) {}
		// Used by GameObject to dispose the components
		virtual void PrepareDestroy(){}
		// Called when the object is destroyed
		virtual void OnDestroy() {}
		// Destroys given object
		static void Destroy(Object* objToDestroy);
		// Destroys this object
		void Destroy();
	};
}
