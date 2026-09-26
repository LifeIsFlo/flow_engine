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
    float3 lightPos : CAMERA_POS;
    float3 localPos: POSITION;
};

cbuffer Object : register(b0)
{
    matrix transform;
    matrix rotation;
};

cbuffer Light : register(b1)
{
    matrix view;
    matrix projection;
    float3 position;
    float padding;
};

PSInput main(VSInput input)
{
    PSInput output;
    matrix viewProjecion = mul(view, projection);
    matrix clipSpace = mul(transform, viewProjecion);

    output.position = mul(float4(input.position, 1), clipSpace);
    output.lightPos = position;
    output.localPos = mul(float4(input.position, 1), transform);
    return output;
}