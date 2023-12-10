
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;
float timeraslow=0;
float waterline=0.6;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
       
    if (Tex.y>waterline)
    {
			
    	Color = tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x),//-((Tex.x-0.5)*(Tex.y-waterline)*2.1)),
    					(waterline - ((Tex.y-waterline)*((waterline+1)-Tex.y)*1.8f))
    					 )
    				 )*(1-(5*(Tex.y-waterline-0.02))); 	
        
    		
    	Color = Color/2;
    				 
    	Color.g = Color.g + 0.1f;
   		Color.r = Color.r + 0.05f;
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