
texture g_txSrcColor;
sampler2D g_samSrcColor;

float xblur=0;
float yblur=0;

static const float BlurWeights[13] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};


float4 Down( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
    float2 tc;
    float alpha;


    for (int i = 0; i < 13; i++)
    {    
    	tc.y = (i-6)*yblur;
    	tc.x = (i-6)*xblur;
        Color += (tex2D( g_samSrcColor, Tex.xy - tc.xy )-0.7) *  BlurWeights[i] * 40;
    }
    
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