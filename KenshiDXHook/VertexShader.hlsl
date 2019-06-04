R""(

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PixelShaderInput main( float4 pos : POSITION, float4 color : COLOR )
{
    PixelShaderInput psi;
    psi.color = color;
    psi.pos = pos;
    return psi;
}

)""