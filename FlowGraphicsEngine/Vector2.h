#pragma once
#include "FWD.h"
namespace FlowEngineDX11 {
	struct Vector2
	{
		float x = 0;
		float y = 0;

		constexpr Vector2() noexcept = default;

		constexpr Vector2(const float _x = 0.0f, const float _y = 0.0f) noexcept
			: x(_x), y(_y){}

		static float Distance(Vector2 first, Vector2 second);

		float Magnitude();

		Vector2 operator +(const Vector2& added) const {
			return {x + added.x, y + added.y};
		}

		Vector2 operator +=(const Vector2& added) const {
			return *this + added;
		}

		Vector2 operator -(const Vector2& removed) const {
			return { x - removed.x, y - removed.y };
		}

		Vector2 operator -=(const Vector2& removed) const {
			return *this - removed;
		}
		//inline static constexpr Vector2 ZERO{ 0.0f, 0.0f };
		//inline static constexpr Vector2 ONE{ 1.0f, 1.0f };
	};
	
}