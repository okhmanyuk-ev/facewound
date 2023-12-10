#ifndef _INCFLYER
#define _INCFLYER

#include "Globals.h"
#include "Landscape.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "GarrysSoundWrapper.h"

struct GVect2
{
	float x, y;
};

class CFlyingMachine
{
public:

	void Draw(CPlayers* p);
	void Init(CPlayers* p,char* model);
	void Movement(CPlayers* p);
	void Accelerate(CPlayers* p);
	void UpdateMyRect(CPlayers* p);
	bool TakeDamage(CPlayers* p, float damage, CPlayers* player);
	LPDIRECT3DTEXTURE9 t_main;
	LPDIRECT3DTEXTURE9 t_afterburner;

	float HeadingAngle;
	GVect2 VelocityVector;
	float Velocity;
	float Gravity;
	float SmokeTimer;
	bool Engine;
	void StartNewLevel(CPlayers* p);
};

#endif