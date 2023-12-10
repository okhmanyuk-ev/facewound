// Entitys.h: interface for the Entitys class.
//
//////////////////////////////////////////////////////////////////////
#ifndef AFX_ENTITYS_H__FE8310AB_4AD8_4AFE_BDF2_FCC225DF63FE__INCLUDED_
#define AFX_ENTITYS_H__FE8310AB_4AD8_4AFE_BDF2_FCC225DF63FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <Windows.h>
#include "garryswrappers/garrysgraphicswrapper.h"

class CGarrysGraphics;
class CLandscape;
class CCore;

#define ENT_SPAWN 1
#define ENTITY_SPAWN 1

#define ENTITY_COMMANDCONSOLE 5

#define ENTITY_TRIGGER 9

struct sSound;


struct sEntitys
{
	bool inuse;

	unsigned int type;
	double timer;
	int soundchannel;

	float x, y;
	float Width, Height, c, d, e, f, g, h, i, j, k, l, m, n;

	char name[20];
	char trigger[200];
	char String1[500];

	GRECT rect;
	LPDIRECT3DTEXTURE9 texture;

	sEntitys* Next;

	sEntitys()
	{
		inuse=false;
		x=0;
		y=0;
		Width=0;
		Height=0;
		timer = 0;
		c=0;
		d=0;
		e=0;
		f=0;
		g=0;
		h=0;
		i=0;
		j=0;
		k=0;
		l=0;
		m=0;
		n=0;
		sprintf(name,"");
		sprintf(trigger,"");
		sprintf(String1,"");
		texture = NULL;
		soundchannel=0;
	}
};

typedef sEntitys* LPEntity;

class CPlayers;


class Entitys  
{
public:
	void DrawEnts(int layer);
	void SetGfxPointer(CGarrysGraphics* gfxpointer);
	LPEntity AddEntity();
	void Init();

	LPEntity FirstEnt;
	unsigned int NumEnts;
	void RemoveAll();

	bool bDraw;

	CCore* pcore;
	CGarrysGraphics* gfx;

	void StartLevelSpawners(void);
	bool PlayerTrigger(CPlayers* player, GRECT* playerrect);
	bool EntityHere(unsigned int entity, float x, float y);
	bool Trigger(char* name);
	void doEnt_Sound(LPEntity p);
	void DoEntityParticleSpawner(LPEntity Pe);
	void DoTriggerOnNoEnemies(LPEntity Pe);
	void DoEntitys(void);
	void DrawLensFlare(LPEntity lp);
	void ProcessEntitys(void);
	void DrawPlutonium(LPEntity lp);
	LPEntity CollidePickup(GRECT r1);
	void RemoveEnt(LPEntity lp);
	LPEntity Previous(LPEntity index);
	void DrawProp(LPEntity lp);
	void DrawParallaxProp(LPEntity lp);
	void DrawReflection(LPEntity lp);
	void DrawSound(LPEntity p);
	void DrawCheckPoint(LPEntity lp);
	void DoCheckPoint(LPEntity lp);


	LPDIRECT3DTEXTURE9 checkpoint;
	void DrawRotatingProp(LPEntity lp);

	sSound* s_hiddenarea;
	sSound* s_checkpoint;

};

#endif // !defined(AFX_ENTITYS_H__FE8310AB_4AD8_4AFE_BDF2_FCC225DF63FE__INCLUDED_)
