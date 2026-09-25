Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
    float3 localPos : POSITION;
    float3 cameraPos : CAMERA_POS;
};

struct LightData
{    
    float3 position;
    float radius;

    float4 color;

    float intensity;
    bool fallOffEnabled;
    float2 padding1;
};

StructuredBuffer<LightData> g_lightData : register(t1);
TextureCubeArray<float> shadowMaps : register(t2);

float3 CalculateLightByIndex(uint lightIndex, PSInput input)
{
    LightData currentLight = g_lightData[lightIndex];
    
    bool obstacle = false;
    
    float3 dir = currentLight.position - input.localPos;
    float dis = length(dir);

    if (!obstacle)
    {
        float3 ray = normalize(dir);
        float theta = acos(saturate(dot(ray, normalize(input.normal))));

        float incidinceAngleProduct = cos(theta);
        float desIntensity = currentLight.intensity;

        if (currentLight.fallOffEnabled)
        {
            if (dis > 0)
            {
                float desDis = dis;
                if (currentLight.radius != 0)
                {
                    desDis = max(0.1, dis - currentLight.radius);
                }
                desIntensity *= (1 / (desDis * desDis));
            }
        }
    
        return (currentLight.color / length(currentLight.color)) * (desIntensity * incidinceAngleProduct);
    }
    return float3(0, 0, 0);
}

float3 CalculateLight(LightData currentLight, PSInput input)
{
    float3 dir = currentLight.position - input.localPos;

    float dis = length(dir);
    float3 ray = normalize(dir);
    float theta = acos(dot(ray, normalize(input.normal)));

    float incidinceAngleProduct = saturate(cos(theta));
    float desIntensity = currentLight.intensity;

    if (currentLight.fallOffEnabled)
    {
        if (dis > 0)
        {
            float desDis = dis;
            if (currentLight.radius != 0)
            {
                desDis = max(0.1, dis - currentLight.radius);
            }
            desIntensity *= (1 / (desDis * desDis));
        }
    }
    
    return (currentLight.color / length(currentLight.color)) * (desIntensity * incidinceAngleProduct);
}


float4 main(PSInput input) : SV_TARGET
{
    float4 baseColor = shaderTexture.Sample(sampleType, input.uv);

    if (length(baseColor) < 0.01f)
    {
        baseColor = float4(1,1,1,1) / 255 * input.color;
    }

    float3 added = float3(0,0,0);

    LightData cameraLight = { { input.cameraPos }, 0, { 1, 1, 1, 1 }, 0.2f, false, {1,1} };
    
    uint numLights = 0;
    uint stride = 0;
    
    g_lightData.GetDimensions(numLights, stride);

    for (uint i = 0; i < numLights; i++)
    {
        added += CalculateLightByIndex(i, input);
    }
    
    added += CalculateLight(cameraLight, input);

    return baseColor * float4(added, 1.0f);
}