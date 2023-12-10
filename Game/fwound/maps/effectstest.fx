
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;
float timeraslow=0;
float waterline=0.6;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
        
    Color = tex2D( g_samSrcColor, Tex.xy);
        
    if (Tex.y>(waterline-0.5))
    {
    	
    if (Tex.y>waterline)
    {
   	
   		Color = tex2D(g_samSrcColor, Tex); 	
   		
    	Tex.y = (waterline - ((Tex.y-waterline)*1.1f));
    				  				
    	//Color = tex2D(g_samSrcColor, Tex); 
    	
    	Color.r += tex2D(g_samSrcColor, Tex)*6.0f; 		
          		
    }
    else
    {
    	// light coming from the lava
    	Color.r = Color.r * (1+(Tex.y-(waterline-0.5f))*5.0f);
    	Color.g = Color.g * (1+(Tex.y-(waterline-0.5f))*3.5f);
   	}
   	
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