
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
    
  //  float fsin = sin((Tex.x+Tex.y+(sinslow*2))*100);
    
    Color = tex2D( g_samSrcColor, Tex+(   fsin  *  (sinslow*0.001f) ) );
 //   Color += tex2D( g_samSrcColor, Tex+(0.003*sinslow)+(   fsin  *  (sinslow*0.001f) ) )-0.1;

    
    
    Color = Color*0.7;
    Color.a = 1.0f;

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