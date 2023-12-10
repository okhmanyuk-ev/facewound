
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;
float timeraslow=0;
float waterline=0.6;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
    float wavegen;
        
    if (Tex.y>waterline)
    {
    	wavegen = sin(((Tex.y-waterline)*250)+timeraslow+Tex.x)*(Tex.y-waterline)*0.05f;
    	
    	Color = tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x-((Tex.x-0.5)*(Tex.y-waterline)*2.8))+wavegen,
    					(waterline - ((Tex.y-waterline)*(waterline+1-Tex.y)*2))+wavegen
    					 )
    				 )*(1-(2*(Tex.y-waterline-0.02)));
    				 
    	// colour the water - this looks best if it's really really slight
    	Color.r += 0.0;
    	Color.g += 0.02;
    	Color.b += 0.01;
    	
    	// contrast - again, slight is better
    	Color = (Color - 0.1) * 1.3;
    	
    	
    }
    else
    {
    	 Color = tex2D( g_samSrcColor, Tex.xy);
   	}

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