struct VSInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

cbuffer Transform : register(b0)
{
    matrix transform;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position = mul(float4(input.position, 0, 1), transform);
    output.uv = input.uv;

    return output;
}