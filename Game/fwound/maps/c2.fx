
texture g_txSrcColor;
sampler2D g_samSrcColor;

float mousex=0.1;
float mousey=0.1;
float size=0.1f;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
    
    
    Color = (tex2D( g_samSrcColor, float2(Tex.x,Tex.y) ) -0.05)*1.2;
    
    Color.a = 0.85;
    return Color;
}


technique PostProcess
{
    pass p1
    {
        VertexShader = null;
        PixelShader = compile ps_2_0 Down();
        ZEnable = false;
    }

}