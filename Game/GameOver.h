#pragma once

#include "cTextures.h"
#include "fmod.h"

class CGameOver
{
public:
	CGameOver(void);
	~CGameOver(void);
	void Do(void);
	void Loop(void);
	void Draw(void);
	void Init(void);
	bool EscapeKeyDown;
	float f_timer;
	CCore* pcore;
	void UnInit(void);
	float GuiScale;
	FSOUND_STREAM* music;
};
