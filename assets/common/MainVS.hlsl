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
    float3 cameraPos : CAMERA_POS;
};

cbuffer Transform : register(b0)
{
    matrix transform;
    matrix rotation;
};

cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
    float3 cameraPos;
    float padding;
};

PSInput main(VSInput input)
{
    PSInput output;

    matrix viewProjecion = mul(view, projection);
    matrix clipSpace = mul(transform, viewProjecion);

    output.position = mul(float4(input.position, 1), clipSpace);

    output.uv = input.uv;
    output.normal = mul(float4(input.normal, 0), rotation);
    output.localPos = mul(float4(input.position, 1), transform);
    output.color = input.color;
    output.cameraPos = cameraPos;

    return output;
}