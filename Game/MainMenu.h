#pragma once

#include "cTextures.h"
#include "options.h"
#include "fmod.h"

class CMainMenu
{
public:

	CMainMenu(void);
	~CMainMenu(void);
	void Do(void);
	void Loop(void);
	void Draw(void);
	void Init(void);
	bool EscapeKeyDown;
	float xscale;
	float yscale;
	float f_timer;
	CCore* pcore;
	COptions options;
	LPDIRECT3DTEXTURE9 t_mainlogo;
	LPDIRECT3DTEXTURE9 t_menubits;
	LPDIRECT3DTEXTURE9 t_menua;
	LPDIRECT3DTEXTURE9 t_menub;
	LPDIRECT3DTEXTURE9 t_glint;
	void DrawGlint(float x, float y);
	void DrawGlintFaded(float x, float y);
	void DrawMainLogo(void);
	int HoveredOver(void);
	void UnInit(void);
	POINT point;
	void onMouseOne(void);
	void DrawButtons(void);
	void DrawTextScroller(void);
	float scrolloffset;
	float GuiScale;
	FSOUND_STREAM* music;
	bool bShowOptions;
	int musicchannel;
	void DrawSky(void);

	float m_skyx;
	LPDIRECT3DTEXTURE9 skymap[16];
	LPDIRECT3DTEXTURE9 picture;
	void LoadSky(char* skyname);
	void DrawPicture(void);
	LPDIRECT3DTEXTURE9 splash[24];
	void DrawSplash(void);
	float SplashTimer;
	float logoy, logoyvel;
	void ResetInVariables(void);
	int SwipeType;
};
