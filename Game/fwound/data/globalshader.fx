texture g_texturea;
texture g_textureb;

sampler2D g_samSrcColor =
sampler_state
{
    Texture = <g_texturea>;
};


sampler2D g_samNrmColor =
sampler_state
{
    Texture = <g_textureb>;
};

float BulletTime=1.0f;
float ScreenMoveX=0.0f;
float ScreenMoveY=0.0f;

float4 MyShader( float2 Tex : TEXCOORD0 ) : COLOR0
{

    float4 Color=0;
    float4 NrmColor=0.5f;
    float alpha;
    
    NrmColor = tex2D( g_samNrmColor, Tex.xy);
    
    Tex.x += (-0.5+(NrmColor.r))*0.3f; 
    Tex.y += (-0.5+(NrmColor.g))*0.3f;
    
   
    
 	Color = tex2D( g_samSrcColor, Tex.xy);
 	
 	if (BulletTime!=1.0f)
 	{
 		Color.rgb = (((Color.r+Color.g+Color.b)/3.0f)*(((1-BulletTime)*0.8))) + (Color.rgb*(1-((1-BulletTime)*0.8)));
 		
 		Color = Color * ((1.0f-BulletTime)+1.0f);
 		Color.rgb = ((Color*Color)*(1-BulletTime)) + (Color*(BulletTime));
 		Color.a = 1-((1-BulletTime)*0.5);
 		
 		
 		Color.a = 1.0f;
 	}
 	else
 	{
 		Color.a = 1.0f;
 	}

 	
 	//Color.a = 1.0f;

    return Color;
}


technique PostProcess
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_2_0 MyShader();
    }

}