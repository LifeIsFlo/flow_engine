#pragma once
#include "Engine.h"
#include <type_traits>
#include <concepts>

namespace FlowEngine {
	class ComponentHolderProtocol
	{
	public:
		std::vector<std::unique_ptr<Component>> _components;
	public:
		template<typename T>
		requires std::derived_from<T, Component>
		T* AddComponent();
	};
}