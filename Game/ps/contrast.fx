
texture g_txSrcColor;
sampler2D g_samSrcColor;

float darken=0.5; // down
float multiply=2; // multiplier

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
    Color = (tex2D( g_samSrcColor, Tex.xy ) - darken) * multiply;
    return Color;
}


technique Contrast
{
    pass p1
    {
        VertexShader = null;
        PixelShader = compile ps_2_0 Down();
        ZEnable = false;
    }
}