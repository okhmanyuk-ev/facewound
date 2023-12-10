#pragma once

#include "cTextures.h"
#include "fmod.h"


class CGameIntro
{
public:
	CGameIntro(void);
	~CGameIntro(void);

	CCore* pcore;
	void doLoop(void);
	bool Do(void);
	void Draw(void);
	void Init(void);
	void UnInit(void);
	float fTimer;
	float GuiScale;
	bool Initiated;
	LPDIRECT3DTEXTURE9 t_mainlogo;
	LPDIRECT3DTEXTURE9 t_palette;
	LPDIRECT3DTEXTURE9 t_subtitle;
	FSOUND_STREAM* music;
	void DrawLogo(float alpha);
};
