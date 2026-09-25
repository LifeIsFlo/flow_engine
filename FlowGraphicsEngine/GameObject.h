#pragma once
#include "FWD.h"
#include "Object.h"
#include "Component.h"
#include "FlowEngine.h"

namespace FlowEngineDX11
{
	class GameObject : public Object
	{
		std::vector<Component*> _components;
	public:
		Transform* transform = nullptr;
		void OnInstantiate(FlowEngine* _engine) override;

		template<typename T>
		requires std::derived_from<T, Component>
		T* AddComponent()
		{
			if (T* comPtr = engine->AddObject<T>()) {
				comPtr->SetGameObject(this);
				_components.push_back(comPtr);
				return comPtr;
			}
			std::cout << "Couldn't add component";
			return nullptr;
		}

		template<typename T>
		requires std::derived_from<T, Component>
		T* GetComponent()
		{
			for (Component* com : _components)
			{
				if (T* desiredCom = dynamic_cast<T*>(com))
				{
					return desiredCom;
				}
			}
			return nullptr;
		}
		void PrepareDestroy() override;
		ObjectRegistry GetRegistry() override;
	};
}
