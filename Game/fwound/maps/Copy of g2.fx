
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
    					(timeraslow/2) +  // make the waves move (multiply this number to make faster/slower)
    					((Tex.y-waterline)*400) // the * number here controls the regularity of the waves
    				)
    				*0.01f
    				*((Tex.y-waterline)*5.0f); // more wavy the closer to the camera
    				
    				
    				
    				
    	wavegencos = cos((timeraslow*0.5)
    					+ ((Tex.y-waterline)*300)
    					)*0.02f*((Tex.y-waterline)*5.0f);
    				
   	
    	Color = tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x)+wavegen,//-((Tex.x-0.5)*(Tex.y-waterline)*2.1)),
    					(waterline - ((Tex.y-waterline)*((waterline+1)-Tex.y)*1.8f))+wavegencos
    					 )
    				 )*(1-(5*(Tex.y-waterline-0.02))); 	
        
        Color += tex2D( 
    				g_samSrcColor, 
    				float2(
    					(Tex.x),
    					(waterline - ((Tex.y-waterline)*((waterline+1)-Tex.y)*1.8f))
    					 )
    				 )*(1-(5*(Tex.y-waterline-0.02)))*0.5; 
    		
    	Color = Color/2;
    				 
    				 
    	//Color.b = Color.b - 0.0f;
   		Color.g = Color.g + 0.1f;
   		Color.r = Color.r + 0.05f;
   		Color = Color * 1.1f;
   		
   		//Color += tex2D( g_samSrcColor, Tex.xy)*0.1; // uncomment to see through the water
   
     		// all this bullshit is to make teh water fade in nicely
   		// there's probably a better way - let me know bugz@facewound.com
   		if (Tex.y<(waterline+0.01f))
   		{
   			Color = (
   					(tex2D( g_samSrcColor, Tex.xy)*(0.01-(Tex.y-waterline))*100)
   					+ (Color*((Tex.y-waterline))*100)
   					) 
   					;
   		}

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