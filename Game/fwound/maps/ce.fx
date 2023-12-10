
texture g_txSrcColor;
sampler2D g_samSrcColor;

float sinslow=0.01;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
     
       
   	Color = tex2D( g_samSrcColor, Tex.xy);
  
    //Color.a = abs(sinslow*50.0f)+0.1f;

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