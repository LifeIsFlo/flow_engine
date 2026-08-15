Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 uvw : TEXCOORD0;
    float3 localPos : TEXCOORD1;
};

float4 main(PSInput input) : SV_TARGET
{
    //float4 textureColor = shaderTexture.Sample(sampleType, input.uvw);
    float gradient = (input.localPos.y + 1.0f) * 0.5f;

    gradient = saturate(gradient);    

    float3 baseColor = float3(0, 1.0f, 1.0f);

    return float4(baseColor * gradient, 1.0f);
}