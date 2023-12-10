// LevelEdit.h: interface for the LevelEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVELEDIT_H__EB4901BC_4036_4CF5_8F99_252CB45D0563__INCLUDED_)
#define AFX_LEVELEDIT_H__EB4901BC_4036_4CF5_8F99_252CB45D0563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "Landscape.h"

#define MAX_ENTITYS 5000

#define ET_SPAWN 1
#define ET_PHYSICSOBJECT 2
#define ET_ZOMBIE 3
#define ET_PROP 4
#define ET_CONSOLECOMMAND 5
#define ET_SOLDIERTURRET 6
#define ET_LADDER 7
#define ET_TRIGGER 9
#define ET_SLIDER 10
#define ET_PICKUP 8
#define ET_PUSH 11

	//if (pLevEdit->entitys[entnumber].type==8) m_typevalue = "ent_pickup";
	//if (pLevEdit->entitys[entnumber].type==9) m_typevalue = "ent_trigger";
	//if (pLevEdit->entitys[entnumber].type==10) m_typevalue = "ent_slider";


// sprop 4
// x y = position - top left
// a b = width/height
// stringa = texturename
// c = breakable
// d = explosion size
// e = spawnparticletype (int)
// f = spawn particle amount (int)

struct sEntitys
{
	bool inuse;
	char name[50];
	unsigned int type;
	float x, y;

	float a, b, c, d, e, f, g, h, i, j, k, l, m, n;

	char stringa[200];
	char stringb[200];
	char stringc[200];
	char stringd[200];
	char stringsma[50];
	char stringsmb[50];
	char stringsmc[50];

	sEntitys()
	{
		inuse=false;
		x=0;
		y=0;
		a=0;
		b=0;
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
		sprintf(name, "");
		sprintf(stringa,"");
		sprintf(stringb,"");
	}
};


struct EntTypes
{
	char name[100];

};


class LevelEdit  
{
	
public:
	void DelEntity();
	void AddEntity();
	bool AllowEdit;
	void SaveMap(const char *filename);
	void FileNew();
	void Setup();
	void Draw();

	void UpdateEntTexture(int num);

	LevelEdit();
	virtual ~LevelEdit();
	CGarrysGraphics gfx;
	CLandscape landscape;
	HWND hWnd;

	EntTypes entitytypes[300];

	unsigned int SelectedEntity;

	LPDIRECT3DTEXTURE9 enttextures[40]; // these are for predefined crap
	LPDIRECT3DTEXTURE9 ptex; 
	

	sEntitys entitys[MAX_ENTITYS];
	LPDIRECT3DTEXTURE9 entitytextures[MAX_ENTITYS]; // each texture has a bmp it can load

	int EditMode;

	//HANDLE hMutex; 

	POINT point;
	POINT rpoint;
	POINT spoint;

	POINT rectpoint;
	bool inrect;

	float ox, oy;
	unsigned int DrawTile;

	void DoTileDrawMode(void);
	float TextureBarScroll;
	void DoEntityMode(void);
	void DoPrefabMode(void);
	void GetRightPath(char* variabletoset);

	int PlutCount;
	int EnemyCount;
	float TimerA;

	void ChangeAlphaMode(bool additive);
	void SetupEntitys(void);
};

#endif // !defined(AFX_LEVELEDIT_H__EB4901BC_4036_4CF5_8F99_252CB45D0563__INCLUDED_)
