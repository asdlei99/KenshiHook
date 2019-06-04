R""(

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PixelShaderInput psi) : SV_TARGET
{
    return psi.color;
}

)""