
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
   	
    	Tex.y = (waterline - ((Tex.y-waterline)*1.1f));
    				  				
    	Color = tex2D(g_samSrcColor, Tex); 	
  
   
        
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y+0.02f)); 
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y-0.01f));
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y-0.005f)); 	
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y+0.005f));
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y+0.01f)); 	
    	Color += tex2D(g_samSrcColor, float2(Tex.x, Tex.y+0.02f)); 
    	Color = Color / 7;
        
        
        Color.rgb = Color.rgb + ((Tex.y-waterline)*0.6f);

   		Color.g = Color.g + 0.1f;
   		Color.r = Color.r + 0.05f;
   		Color = Color * 1.0f;
   		
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