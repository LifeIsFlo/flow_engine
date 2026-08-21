Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
    float3 localPos : POSITION;
};

struct LightData
{
    float3 position;
    float radius;
    float intensity;
    float3 color;
};

StructuredBuffer<LightData> g_lightData : register(t1);


float4 main(PSInput input) : SV_TARGET
{
    float4 baseColor = shaderTexture.Sample(sampleType, input.uv);

    if (length(baseColor) < 0.01f)
    {
        baseColor = float4(1,1,1,1) / 255 * input.color;
    }

    float3 poss[2] = {float3(3,3.0f,3), float3(-3,4.1f, -3)};

    float added = 0;

    for (int i = 0; i < 1; i++)
    {
        float3 dir = poss[i] - input.localPos;

        float dis = length(dir);
        float3 ray = normalize(dir);
        float theta = acos(dot(ray, input.normal));

        added += saturate(cos(theta)) / dis * 2;
    }

    return baseColor * (added * float4(1.0f, 0.7f, 0, 1.0f));
}