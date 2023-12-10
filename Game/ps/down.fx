
texture g_txSrcColor;
sampler2D g_samSrcColor;


float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
    float2 tc;


    for (int i = 0; i < 13; i++)
    {    
    	tc.x = 0;
    	tc.y = (i-7)*0.002;
        Color += tex2D( g_samSrcColor, (Tex.xy*16) + tc.xy);
    }
    
    for (int i = 0; i < 13; i++)
    {    
    	tc.y = 0;
    	tc.x = (i-7)*0.002;
        Color += tex2D( g_samSrcColor, (Tex.xy*16) + tc.xy);
    }
    
    return Color/26;
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