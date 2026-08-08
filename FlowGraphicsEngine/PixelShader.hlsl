Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct VSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

float4 main(VSInput input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(sampleType, input.uv);

    //float4 textureColor = float4(1, 0, 0, 1);

    return textureColor;
}