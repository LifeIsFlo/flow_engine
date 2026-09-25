#pragma once
#include <d3d11.h>

#include "Component.h"
#include "Color.h"
#include "FWD.h"
#include "LightData.h"
#include "ShadowMap.h"

namespace FlowEngineDX11
{
	class Light : public Component
	{
		ID3D11Buffer* viewProjectionBuffer = nullptr;
		std::unique_ptr<ShadowMap> shadowMap;
	public:
		Color color{};
		float radius = 3;
		float intensity = 1;
		bool falloffEnabled = true;
		~Light();
		void ShadowPass(const DX11Protocol* dx11_protocol, const LightingProtocol* lighting_protocol);
		HRESULT CreateResources(const DX11Protocol* dx11_protocol);
		LightData GetLightData();
		ShadowMap* GetShadowMap();
		ObjectRegistry GetRegistry() override;
	};
}
