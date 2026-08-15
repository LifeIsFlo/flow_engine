#pragma once
#include "FWD.h"
namespace FlowEngineDX11 {
	struct Vector2
	{
		float x = 0;
		float y = 0;

		constexpr Vector2() noexcept = default;

		constexpr Vector2(const float _x, const float _y) noexcept
			: x(_x), y(_y){}

		static float Distance(Vector2 first, Vector2 second);

		float Magnitude();

		Vector2 operator+(const Vector2& added) const {
			return { x + added.x, y + added.y };
		}

		Vector2 operator+=(const Vector2& added) {
			x += added.x;
			y += added.y;
			return *this;
		}

		Vector2 operator-(const Vector2& removed) const {
			return { x - removed.x, y - removed.y };
		}

		Vector2 operator-=(const Vector2& removed) {
			x -= removed.x;
			y -= removed.y;
			return *this;
		}
		static const Vector2 ZERO;
		static const Vector2 ONE;
	};
	
}