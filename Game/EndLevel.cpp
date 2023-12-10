#include "EndLevel.h"
#include "core.h"

#define GFX this->pcore->gfx

#define SCRW (GFX.m_d3dpp.BackBufferWidth)
#define SCRH (GFX.m_d3dpp.BackBufferHeight)
#define SCALE (pcore->ScreenZoom)

CEndLevel::CEndLevel(void)
{
}

CEndLevel::~CEndLevel(void)
{
}

void CEndLevel::Do(void)
{
	pcore->gfx.UpdateTimer();
	pcore->gfx.InputRefreshKeyboard();
	pcore->gfx.InputRefreshMouse();

	if (pcore->gfx.m_bMouse1)
	{
		this->UnInit();
		pcore->StartNextLevel(pcore->landscape.m_NextLevel);
	}
}

void CEndLevel::Loop(void)
{
	Draw();
	Do();
}

void CEndLevel::Draw(void)
{
	GFX.Begin();
	pcore->SetAlphaMode(false);
	DrawSky(255, 0.0f);
	pcore->SetAlphaMode(true);

	float *spectrum;
	float multiplier =0;
    spectrum = FSOUND_DSP_GetSpectrum(); 
	if (spectrum)
	{
		for (int i=100;i<512;i++)
		{
			multiplier = multiplier + spectrum[i];
		}
		multiplier = multiplier * 30.0f;
		if (multiplier>255) multiplier = 255;
		if (multiplier<0) multiplier = 0;
	}

	
	DrawSky(multiplier, 0.0f);
	pcore->SetAlphaMode(false);

	GFX.SpriteDraw(t_title,(SCRW/2)-(256*SCALE),20*SCALE,0,0,0,SCALE, SCALE);

	DoneCounting = false;
	DrawCounter(&this->cKills);
	DrawCounter(&this->cPlutonium);

	DrawCounter(&this->cBulletsFired);
	DrawCounter(&this->cBulletsLanded);
	DrawCounter(&this->cAccuracy);
	DrawCounter(&this->cSecrets);

	pcore->landscape.DrawScreenFade();
	GFX.End();
	GFX.Flip();
}

void CEndLevel::Init(void)
{
	LoadSky("levelend");
	t_title = pcore->textures.LoadTexture("textures/gui/endlev/levelend.tga");
	t_c		= pcore->textures.LoadTexture("textures/gui/endlev/c.tga");
	t_Numbers = pcore->textures.LoadTexture("textures/gui/endlev/numbers.tga");

	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);

	sprintf(cKills.name, "Murders");
	cKills.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/cmurders.tga");
	cKills.total = pcore->LevelStats.Enemies;
	cKills.x = 150*SCALE;
	cKills.y = 250*SCALE;
	cKills.sound = pcore->sound.LoadSound(3,"sound/gui/ckill.ogg");
	cKills.timer = -150;
	cKills.speed = 0.1f;
	cKills.done = false;
	cKills.current = 0;

	sprintf(cPlutonium.name, "Plutonium");
	cPlutonium.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/cplutonium.tga");
	cPlutonium.total = pcore->LevelStats.Plutonium;
	cPlutonium.x = 150*SCALE;
	cPlutonium.y = 300*SCALE;
	cPlutonium.sound = pcore->sound.LoadSound(3,"sound/gui/cplut.ogg");
	cPlutonium.timer = -400;
	cPlutonium.speed = 0.9f;
	cPlutonium.done = false;
	cPlutonium.current = 0;

	sprintf(cBulletsFired.name, "Shots Fired");
	cBulletsFired.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/cshots.tga");
	cBulletsFired.total = pcore->LevelStats.ShotsFired;
	cBulletsFired.x = 150*SCALE;
	cBulletsFired.y = 375*SCALE;
	cBulletsFired.sound = pcore->sound.LoadSound(3,"sound/gui/cshots.ogg");
	cBulletsFired.timer = -200;
	cBulletsFired.speed = 1.2f;
	cBulletsFired.done = false;
	cBulletsFired.current = 0;

	sprintf(cBulletsLanded.name, "Shots Landed");
	cBulletsLanded.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/chits.tga");
	cBulletsLanded.total = pcore->LevelStats.ShotsLanded;
	cBulletsLanded.x = 150*SCALE;
	cBulletsLanded.y = 425*SCALE;
	cBulletsLanded.sound = pcore->sound.LoadSound(3,"sound/gui/chits.ogg");
	cBulletsLanded.timer = -200;
	cBulletsLanded.speed = 0.6f;
	cBulletsLanded.done = false;
	cBulletsLanded.current = 0;


	

	sprintf(cAccuracy.name, "Accuracy");
	cAccuracy.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/caccuracy.tga");
	if (pcore->LevelStats.ShotsFired==0)
	{
		cAccuracy.total = 50;
	}
	else
	{
		cAccuracy.total = (int)(((float)pcore->LevelStats.ShotsLanded/pcore->LevelStats.ShotsFired)*100.0f);
	}
	cAccuracy.x = 150*SCALE;
	cAccuracy.y = 475*SCALE;
	cAccuracy.sound = pcore->sound.LoadSound(3,"sound/gui/caccuracy.ogg");
	cAccuracy.timer = -30000;
	cAccuracy.speed = 100.0f;
	cAccuracy.done = false;
	cAccuracy.current = 0;


	sprintf(cSecrets.name, "Secret Areas");
	cSecrets.t_texture = pcore->textures.LoadTexture("textures/gui/endlev/csecrets.tga");
	cSecrets.total = pcore->LevelStats.HiddenAreas;
	cSecrets.x = 150*SCALE;
	cSecrets.y = 525*SCALE;
	cSecrets.sound = pcore->sound.LoadSound(3,"sound/gui/chits.ogg");
	cSecrets.timer = -1.5;
	cSecrets.speed = 0.01f;
	cSecrets.done = false;
	cSecrets.current = 0;
	cSecrets.iof = pcore->landscape.HiddenAreas;

	s_counter = pcore->sound.LoadSound(3,"sound/gui/ccount.ogg");
}


void CEndLevel::UnInit(void)
{
	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), FALSE);
}

void CEndLevel::DrawSky(float alpha, float offset)
{

	float sc = ((pcore->landscape.window_width)/256.0f);
	float czoom = 1;

	if (pcore->landscape.window_height>pcore->landscape.window_width) sc = ((pcore->landscape.window_height/3.0f)/256.0f);
	if (pcore->landscape.window_height/3>256*sc) sc = ((pcore->landscape.window_height/3.0f)/256.0f);

	if (sc<1) sc=1;
	int x,y;
	int i=0;

	m_skyx = m_skyx	- (0.5f*(pcore->gfx.RealTick));

	if (m_skyx>sc*4*256) m_skyx=0;
	if (m_skyx<0) m_skyx=m_skyx+(sc*4*256);
	int iadd = (int)(((((float)m_skyx+offset))/(256*sc)))%4;

	for (y=0;y<6;y++)
	{

		for (x=0;x<1;x++)
		{

			pcore->gfx.SpriteDrawEx(skymap[( (y+iadd) )%4],
						
						(x*sc*256),
						(y*sc*256)-(fmod(m_skyx+offset,256*sc))+offset,
						
						

						0,0,0,
						
						(sc*czoom),(sc*czoom),
						0,0,256,256,
						alpha,255,255,255
						
						
						
						);
		}
	}

	i=0;

}

void CEndLevel::LoadSky(char* skyname)
{
	HRESULT hr;	
	char filename[500];
	for (int i=1;i<=4;i++)
	{
		sprintf(filename, "textures/skys/%s/%i.jpg",skyname,i);
		skymap[i-1] = pcore->textures.LoadTexture(filename);
	}
}

void CEndLevel::DrawCounter(SCounter *c)
{
	
	pcore->gfx.SpriteDrawEx(t_c,c->x-(5*SCALE), c->y-(2*SCALE), 0,0,0,SCALE, SCALE,50,255,255,255);
	pcore->gfx.SpriteDraw(c->t_texture,c->x, c->y, 0,0,0,SCALE, SCALE);
	DrawNumber(c->current,c->x+ (150*SCALE), c->y,0.9f);

	if (c->iof!=0)
	{
		DrawNumber(c->iof,c->x+ (220*SCALE), c->y+ (10*SCALE),0.6f);
	}

	if (c->done) return;

	if (!DoneCounting)
	{
		c->timer = c->timer + c->speed*pcore->gfx.RealTick;
		if (c->current!=(int)c->timer && c->timer>0)
		{
			pcore->sound.PlaySnd(s_counter,0,0,0,-5.0f);
		}
		c->current = (int)c->timer;
		if (c->current<0) c->current = 0;
		if (c->current>=c->total)
		{
			c->current = c->total;
			c->done = true;
				pcore->sound.PlaySnd(c->sound,0,0,0,-5.0f);
		}
		DoneCounting = true;
	}

}

void CEndLevel::DrawNumber(int number, int x, int y, float scale)
{
	char buffer[20], bufferchar[2];
	bufferchar[1] = '\0';
	sprintf(buffer,"%i\0", number);
	int i, num;
	i=0;
	while (i<strlen(buffer))
	{
		bufferchar[0] = buffer[i];
		num = atoi(bufferchar);
		pcore->gfx.SpriteDrawEx(t_Numbers, x+(i*20*scale*SCALE),y,0,0,0,SCALE*scale,SCALE*scale,num*26,0,26,32,255,255,255,255);
		i++;
	}

}