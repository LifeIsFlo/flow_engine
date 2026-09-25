#pragma once
#include <cstdint>
#include <array>

namespace FlowEngineDX11 {
	struct Color {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		static Color FromHEX(const uint32_t hexRGBA) {
			uint8_t r = (hexRGBA >> 24) & 0xFF;
			uint8_t g = (hexRGBA >> 16) & 0xFF;
			uint8_t b = (hexRGBA >> 8) & 0xFF;
			uint8_t a = hexRGBA & 0xFF;
			return { r,g,b,a };
		}

		uint32_t ToHex() const {
			return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
		}

		std::array<float, 4> ToFloatArray() const
		{
			std::array floats { 1.0f / 255 * static_cast<float>(r), 1.0f / 255 * static_cast<float>(g), 1.0f / 255 * static_cast<float>(b), 1.0f / 255 * static_cast<float>(a) };
			return floats;
		}

		Color(const uint8_t r = 255, const uint8_t g = 255, const uint8_t b = 255, const uint8_t a = 255) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}


		Color operator/(const float& divide) const 
		{
			auto retR = static_cast<uint8_t>(static_cast<float>(r) / divide);
			auto retG = static_cast<uint8_t>(static_cast<float>(g) / divide);
			auto retB = static_cast<uint8_t>(static_cast<float>(b) / divide);
			auto retA = static_cast<uint8_t>(static_cast<float>(a) / divide);

			return { retR, retG, retB, retA };
		}
	};

	struct GPUColor {
		float r;
		float g;
		float b;
		float a;

		GPUColor(const Color& color = Color()) {
			auto floats = color.ToFloatArray();
			r = floats[0];
			g = floats[1];
			b = floats[2];
			a = floats[3];
		}
	};
}