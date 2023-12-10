#pragma once

#include "cTextures.h"
#include "fmod.h"
#include "GarrysSoundWrapper.h"

struct SCounter
{
	sSound* sound;
	char name[100];
	bool done;
	LPDIRECT3DTEXTURE9 t_texture;
	float timer;
	int total;
	int current;
	float x, y;
	float speed;
	int iof;
	SCounter()
	{
		sound=NULL;
		t_texture=NULL;
		timer = -100;
		current  = 0;
		done = false;
		x = 10;
		y = 10;
		speed = 0.4f;
		iof = 0;
	}
};

class CEndLevel
{
public:
	CEndLevel(void);
	~CEndLevel(void);
	void Do(void);
	void Loop(void);
	void Draw(void);
	void Init(void);
	CCore* pcore;
	void UnInit(void);
	FSOUND_STREAM* music;
	int musicchannel;
	void DrawSky(float alpha, float offset);
	float m_skyx;
	LPDIRECT3DTEXTURE9 skymap[4];
	void LoadSky(char* skyname);

	LPDIRECT3DTEXTURE9 t_title;
	LPDIRECT3DTEXTURE9 t_c;
	LPDIRECT3DTEXTURE9 t_Numbers;


	SCounter cKills;
	SCounter cPlutonium;

	SCounter cBulletsFired;
	SCounter cBulletsLanded;
	SCounter cAccuracy;
	SCounter cSecrets;

	void DrawCounter( SCounter *c);
	bool DoneCounting;
	void DrawNumber(int number, int x, int y, float scale);

	sSound* s_counter;
};
