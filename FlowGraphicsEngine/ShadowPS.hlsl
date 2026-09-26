struct PSInput
{
    float4 position : SV_POSITION;
    float3 lightPos : CAMERA_POS;
    float3 localPos : POSITION;
};

float main(PSInput input) : SV_Depth
{
    float distanceFromLight =
        length(input.localPos - input.lightPos);

    return distanceFromLight / 50.0f;
}