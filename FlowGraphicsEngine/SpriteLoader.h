#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {

	class SpriteLoader
	{
	public:
		static void LoadSprite(const std::string& assetLocation, std::map<std::string, std::unique_ptr<Sprite>>& spritesOut);
		static void GetArtificialColorData(std::vector<Color>& colors, const UINT& width, const UINT& height);
	};

}