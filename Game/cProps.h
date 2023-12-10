#pragma once

#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "landscape.h"


#define PICKUPTYPE_PLUTONIUM 1001

#define PTYPE_PROP 1
#define PTYPE_DESTROYABLE 2
#define PTYPE_MOVER 3
#define PTYPE_PICKUP 4

struct sProps
{
	char name[20];
	char trigger[20];

	bool canbedestroyed;
	bool ispickup;

	bool MoverActivated;

	unsigned int pickuptype;
	unsigned int type;
	float x, y;
	float health;
	float explosion;
	float xtarget, ytarget;
	float xoff, yoff;
	float timer1;
	bool ElevatorOut;

	float width, height;

	int particle;
	int particle_num;
	int blendmode;
	GRECT mover_rect; 
	GRECT colrect;

	float xvel,yvel;

	LPDIRECT3DTEXTURE9 texture;

	sProps* Next;

	sProps()
	{
		ElevatorOut=false;
		sprintf(trigger,"");
		xvel = 0.0f;
		yvel = 0.0f;
		MoverActivated=false;
		xoff = 0;
		yoff = 0;
		timer1=0;
		pickuptype = 0;
		ispickup = false;
		Next=NULL;
		x=0;y=0;
		health=100;
		explosion=0;
		particle=0;
		particle_num=0;
		canbedestroyed=0;
		texture=NULL;
		width=32.0f;
		height=32.0f;
		blendmode=0;
		type = 1;
	}
};

class CCore;

class cProps
{
public:
	cProps(void);
	~cProps(void);

	void Init();
	void DrawProps(unsigned int layer);
	
	unsigned int NumProps;

	sProps* AddProp();
	sProps* firstprop;

	CCore* pcore;

	bool Collision(float x, float y);
	bool Collision(RECT* rct);
	bool Collision(GRECT* rct);
	
	bool BulletHitProp(int x, int y, LPParticle bullet);
	bool SplashtHitProp(float x, float y, float size, float power);
	void RemoveProp(sProps* prop);
	void RemoveAll();

	sProps* Previous(sProps* index);
	void KillProp(sProps* p, float attack_angle, float speed);
	sProps* CollidePickup(GRECT rc);
	
	void UpdateProp(sProps* p);

	sSound* s_pickup_plutonium;
	void ActivateMover(sProps* p);
	void DoMover(sProps* p);
	bool Trigger(char* name);
};
