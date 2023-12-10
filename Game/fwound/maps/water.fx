
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
    float wavegencos;
        
    if (Tex.y>waterline)
    {
    	wavegen = sin(			
    					timeraslow +  // make the waves move (multiply this number to make faster/slower)
    					((Tex.y-waterline)*300) // the * number here controls the regularity of the waves
    				)
    				*0.01f
    				*((Tex.y-waterline)*5.0f); // more wavy the closer to the camera
    				
    				
    				
    				
    	wavegencos = cos((timeraslow*0.5)
    					+ ((Tex.y-waterline)*300)
    					)*0.02f*((Tex.y-waterline)*5.0f);
    				
   	
    	Color = tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x-((Tex.x-0.5)*(Tex.y-waterline)*2.1))+wavegen,
    					(waterline - ((Tex.y-waterline)*((waterline+1)-Tex.y)*3.8f))+wavegencos
    					 )
    				 )*(1-(5*(Tex.y-waterline-0.02))); 	
    	Color.b = Color.b + 0.04f;
   		Color.g = Color.g + 0.02f;
   		Color = Color * 1.1f;

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