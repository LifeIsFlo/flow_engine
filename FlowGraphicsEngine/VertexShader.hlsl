struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    uint4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
    float3 localPos : POSITION;
};

cbuffer Transform : register(b0)
{
    matrix transform;
};

cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
};

PSInput main(VSInput input)
{
    PSInput output;

    matrix viewProjecion = mul(view, projection);
    matrix clipSpace = mul(transform, viewProjecion);

    output.position = mul(float4(input.position, 1), clipSpace);

    output.uv = input.uv;
    output.normal = input.normal;
    output.localPos = mul(float4(input.position, 1), transform);
    output.color = input.color;

    return output;
}