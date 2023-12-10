#pragma once

#include "landscape.h"

class CCore;
class enemy_base;
//typedef void* LPParticle;
#define ENEMY_ZOMBIE 3
#define ENEMY_SOLDIERTURRET 6


enum {AI_WAIT, AI_JUMP, AI_FALL, AI_LEFT, AI_RIGHT};

struct sEnemies
{
	unsigned int type;
	enemy_base* pointer;

	sEnemies* Next;

	sEnemies()
	{
		type=0;
	}
};

typedef sEnemies* LPEnemy;

class CEnemies
{
public:

	CEnemies(void);
	~CEnemies(void);

	void DrawEnemies(int layer);
	void DoEnemies(void);
	LPEnemy AddEnemy(unsigned int type, float x, float y, float a, float b, char* str1, char* str2);

	sEnemies* FirstEnemy;
	sEnemies* LastEnemy;

	CCore* pcore;

	unsigned int NumEnemies;
	unsigned int NumEnemiesDrawn;
	unsigned int NumEnemiesActivated;

	enemy_base* enemy[10];
	void RemoveEnemy(LPEnemy find);
	void RemoveAll();
	bool BulletHitEnemy(bool DoDamage, float x, float y, LPParticle bullet);
	bool BulletHitEnemyRect(GRECT* rectangle, LPParticle bullet);
	LPEnemy CEnemies::PreviousNME(LPEnemy index);
	bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker);
	float GetNearestEnemy(float x, float y, LPEnemy* enemy, float* xdist, float* ydist, LPEnemy notthisenemy, int nottype);


	void Init(void);
};


class enemy_base
{
	public:
	CEnemies* pCEnemies;
	float health;
	GRECT my_rect;
	virtual void Do(void){};
	virtual void Draw(int layer){};
	virtual void DrawNormalMap(){};
	virtual bool BulletHit(float* x, float* y){return false;};
	virtual bool BulletHitRect(GRECT* rectangle){return false;};
	virtual bool LoadStuff(void){return true;}
	virtual void TakeDamage(float damage, float angle, float x, float y, bool Splash=false, LPParticle bullet=NULL){};
	virtual void SetXY(float x, float y){};
	virtual void SetUp(float ix, float iy, float a, float b, char* string1, char* string2){};
	virtual float GetX(){return 0;};
	virtual float GetY(){return 0;};
	virtual float GetFacing(){return 1;};
	virtual bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker){return false;};
};