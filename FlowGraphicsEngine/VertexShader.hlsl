struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 localPos : POSITION;
};

cbuffer Transform : register(b0)
{
    matrix transform;
};

PSInput main(VSInput input)
{
    PSInput output;

    output.position = mul(float4(input.position, 1), transform);
    output.uv = input.uv;
    output.normal = input.normal;
    output.localPos = output.position;

    return output;
}