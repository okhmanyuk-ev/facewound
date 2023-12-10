
texture g_txSrcColor;
sampler2D g_samSrcColor;

float rnum=1;

float sinslow=0.01;
float timeraslow=0;
float offsetx=2.1;

float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color;
    
   	Color = tex2D( g_samSrcColor, Tex.xy);
   	// uncomment for crazy zoom effect
   	/*
	Color = Color + tex2D( g_samSrcColor, (Tex.xy*0.99)+0.005)*0.8;
	Color = Color + tex2D( g_samSrcColor, (Tex.xy*0.98)+0.01)*0.6;
	Color = Color + tex2D( g_samSrcColor, (Tex.xy*0.97)+0.015)*0.4;
	Color = Color + tex2D( g_samSrcColor, (Tex.xy*0.95)+0.025)*0.2;
	Color = Color*0.2;
	*/
	
	Color = Color*0.8;
    
   	//Color = Color + 0.1; // gives foggy feel
   	Color = Color + Tex.y*0.5;
   	Color.b = Color.b - Tex.y*0.05; // yellow tinge
   	
   	Color = Color-0.2;
   	Color = Color*1.2;
   	
   
	
	
	

	
   

	Color.a = 0.80f;
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