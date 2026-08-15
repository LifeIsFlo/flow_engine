#pragma once
#include "Vector2.h"
#include "FWD.h"


namespace FlowEngineDX11 {
	struct Vector3
	{
		float x = 0, y = 0, z = 0;

		constexpr Vector3() noexcept = default;

		constexpr Vector3(const float& _x, const float& _y) noexcept :
			x(_x), y(_y) {}

		constexpr Vector3(const float& _x, const float& _y, const float& _z) noexcept :
			x(_x), y(_y), z(_z) {}

		float mag();
		void normalize();

		Vector3 operator+=(const Vector3& added)
		{
			x += added.x;
			y += added.y;
			z += added.z;
			return *this;
		}

		Vector3 operator+(const Vector3& added) const
		{
			return { x + added.x, y + added.y, z + added.z };
		}


		Vector3 operator-=(const Vector3& removed)
		{
			x -= removed.x;
			y -= removed.y;
			z -= removed.z;
			return *this;
		}

		Vector3 operator-(const Vector3& removed) const
		{
			return { x - removed.x, y - removed.y, z - removed.z };
		}

		Vector3 operator*(const Vector3& mul) const
		{
			return { x * mul.x, y * mul.y, z * mul.z };
		}

		// Vector2 operations
		Vector3 operator+=(const Vector2& added)
		{
			x += added.x;
			y += added.y;
			return *this;
		}

		Vector3 operator+(const Vector2& added) const
		{
			return { x + added.x, y + added.y, z };
		}

		Vector3 operator-=(const Vector2& removed)
		{
			x -= removed.x;
			y -= removed.y;
			return *this;
		}

		Vector3 operator-(const Vector2& removed) const
		{
			return { x - removed.x, y - removed.y, z };
		}

		// float operations
		Vector3 operator*(const float& mul)
		{
			return { x * mul, y * mul, z * mul };
		}

		Vector3 operator/(const float& div)
		{
			return { x / div, y / div, z / div };
		}

		// conversions
		std::string ToString()
		{
			return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		}

		// Constants
		static const Vector3 ZERO;
		static const Vector3 ONE;
		static const Vector3 RIGHT;
		static const Vector3 FORWARD;
		static const Vector3 UP;
	};
}
