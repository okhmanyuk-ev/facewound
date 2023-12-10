#include "mainmenu.h"
#include "core.h"

#define GFX this->pcore->gfx

CGameOver::CGameOver(void)
{
}

CGameOver::~CGameOver(void)
{
}

void CGameOver::Do(void)
{
	GFX.InputRefreshKeyboard();
	GFX.InputRefreshMouse();
	GFX.UpdateTimer();

	if (!GFX.InputKeyDown(DIK_ESCAPE) &&
		!GFX.InputKeyDown(DIK_SPACE) &&
		!GFX.InputKeyDown(DIK_RETURN))
	{
		EscapeKeyDown = false;
	}


	if ((GFX.InputKeyDown(DIK_ESCAPE) || GFX.InputKeyDown(DIK_SPACE) || GFX.InputKeyDown(DIK_RETURN)) && !EscapeKeyDown)
	{
		this->UnInit();
		pcore->toMenuScreen();
	}

	f_timer += GFX.tick/1000.0f;
}

void CGameOver::Loop(void)
{
	Draw();
	Do();
}

void CGameOver::Draw(void)
{
	GFX.Begin();

	GFX.Clear(200,200,200);
	GFX.DrawText(20,20,"Game Over!",0,0,0);

	GFX.End();
	GFX.Flip();
}

void CGameOver::Init(void)
{
	GuiScale = (pcore->gfx.m_d3dpp.BackBufferWidth+pcore->gfx.m_d3dpp.BackBufferHeight)/((float)1024+768);
	f_timer = 0;
	GFX.InputRefreshKeyboard();

	if (GFX.InputKeyDown(DIK_ESCAPE) ||
		GFX.InputKeyDown(DIK_SPACE) ||
		GFX.InputKeyDown(DIK_RETURN))
	{
		EscapeKeyDown = true;
	}

	music = pcore->sound.LoadStream("sound/music/gameover.ogg",true);
	pcore->sound.PlayStream(music);
}

void CGameOver::UnInit(void)
{
	pcore->sound.CloseStream(music);
	music=NULL;
}