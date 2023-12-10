
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;
float timeraslow=0;
float waterline=0.6;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
    float wavegeny=0;
    float wavegenx=0;
        
    Color = tex2D( g_samSrcColor, Tex.xy );
    
    if (Tex.y>waterline)
    {
    	wavegeny = 0;//sin(((Tex.y+sinslow)*500)+(Tex.x*100))   *(Tex.y-waterline)*0.1;
    	wavegenx = sin(((Tex.y-waterline)*150)+timeraslow+Tex.x)*(Tex.y-waterline)*0.05f;
    	
    	Color = tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x-((Tex.x-0.5)*(Tex.y-waterline)*1.8))+wavegenx,
    					(waterline - ((Tex.y-waterline)*(waterline+1-Tex.y)*5*waterline))+wavegenx
    					 )
    				 )*(1-(3*(Tex.y-waterline-0.02)));
    				 
    	Color += tex2D( g_samSrcColor, Tex.xy );
    	
    	Color=Color/2;
    }

	// 0.4 -

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