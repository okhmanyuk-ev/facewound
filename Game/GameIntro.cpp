#include "gameintro.h"
#include "core.h"

#define GFX this->pcore->gfx

CGameIntro::CGameIntro(void)
{
	fTimer = 0;
	Initiated = false;
}

CGameIntro::~CGameIntro(void)
{
}

void CGameIntro::doLoop(void)
{
	if (!Initiated) Init();

	if (Do()) Draw();
}

bool CGameIntro::Do(void)
{
	GFX.InputRefreshKeyboard();
	GFX.InputRefreshMouse();
	GFX.UpdateTimer();

	fTimer = fTimer + GFX.RealTick;

	if (GFX.InputKeyDown(DIK_SPACE) || GFX.m_bMouse1 || GFX.m_bMouse2 || GFX.InputKeyDown(DIK_ESCAPE) || GFX.InputKeyDown(DIK_RETURN) || fTimer>10000.0f) 
	{
		UnInit();
		return false;
	}
	return true;
}

void CGameIntro::Draw(void)
{
	GFX.Begin();
	GFX.Clear(255,255,255);
	DrawLogo(255);


	float ranxscale = rand()%200==0 ? rand()%2000/5000.0f : 0;
/*
	GFX.SpriteDraw(t_subtitle,
					(GFX.m_d3dpp.BackBufferWidth/2.0f)-(100*GuiScale),
					(GFX.m_d3dpp.BackBufferHeight/2.0f),GuiScale,GuiScale);
*/
	if (fTimer<3500)
	{
		float alpha;
		alpha = 255-255*((fTimer-500)/3000.0f);
		if (alpha<0) alpha = 0;
		if (alpha>255) alpha = 255;
		GFX.SpriteDrawEx(t_palette,
				   0,0,0,0,0,GFX.m_d3dpp.BackBufferWidth,GFX.m_d3dpp.BackBufferHeight,0,0,1,1,
				   alpha,255,255,255);
	}

	if (fTimer>7000)
	{
		float alpha;
		alpha = 255*((fTimer-7000)/1000.0f);
		if (alpha<0) alpha = 255;
		if (alpha>255) alpha = 255;
		GFX.SpriteDrawEx(t_palette,
				   0,0,0,0,0,GFX.m_d3dpp.BackBufferWidth,GFX.m_d3dpp.BackBufferHeight,0,0,1,1,
				   alpha,255,255,255);
	}



	GFX.End();
	GFX.Flip();
}

void CGameIntro::Init(void)
{
	fTimer = 0;
	t_mainlogo = pcore->textures.LoadTexture("textures/menu/facepunch.jpg");
	t_palette = pcore->textures.LoadTexture("textures/menu/palette.tga");
//	t_subtitle = pcore->textures.LoadTexture("textures/menu/facepunch-subtitle.bmp");
	Initiated=true;
	//ShowCursor(false);
	GuiScale = (pcore->gfx.m_d3dpp.BackBufferWidth+pcore->gfx.m_d3dpp.BackBufferHeight)/((float)1024+768);

	music = pcore->sound.LoadStream("sound/music/facepunch.ogg", false);
	pcore->sound.PlayStream(music);

}

void CGameIntro::UnInit(void)
{
	// todo: release old textures
	pcore->sound.CloseStream(music);
	music=NULL;
	pcore->toMenuScreen();
	Initiated=false;
}

void CGameIntro::DrawLogo(float alpha)
{
	float randx=1.0f, randy=1.0f;
	randx = 1.0f + ((rand()%10000)/1000000.0f);
	randy = 1.0f + ((rand()%10000)/1000000.0f);
	if (rand()%200==0) randx = 1.0f + ((rand()%10000)/30000.0f);
	if (rand()%200==0) randy = 1.0f + ((rand()%10000)/30000.0f);
	randx = 1;//randx + (fTimer/100000.0f);
	randy = 1;//randy + (fTimer/100000.0f);
	GFX.SpriteDrawEx(t_mainlogo,
				   (GFX.m_d3dpp.BackBufferWidth/2.0f)-(randx*0.5f*512*GuiScale),
				   (GFX.m_d3dpp.BackBufferHeight/3.0f)-(randy*0.5f*256*GuiScale),0,0,0,
				   randx*GuiScale,randy*GuiScale,
				   0,0,512,512,255,255,255,255);
}
