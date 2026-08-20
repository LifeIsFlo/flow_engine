Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 localPos : POSITION;
};


float4 main(PSInput input) : SV_TARGET
{
    float4 baseColor = shaderTexture.Sample(sampleType, input.uv);

    return baseColor;
}