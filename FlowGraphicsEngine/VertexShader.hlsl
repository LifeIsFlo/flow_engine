struct VSInput
{
    float3 position : POSITION;
    float3 uvw : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 uvw : TEXCOORD0;
    float3 localPos : TEXCOORD1;
};

cbuffer Transform : register(b0)
{
    matrix transform;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position = mul(float4(input.position, 1), transform);
    output.uvw = input.uvw;
    output.localPos = input.position;

    return output;
}