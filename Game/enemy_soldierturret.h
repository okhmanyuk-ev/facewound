#pragma once
#include "enemies.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "core.h"

class enemy_soldierturret : public enemy_base
{
public:
	enemy_soldierturret(void);
	~enemy_soldierturret(void);
	void Draw(int layer);
	void SetUp(float ix, float iy, float a, float b, char* string1, char* string2);
	bool LoadStuff(void);
	void Do(void);
	void TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet);
	bool BulletHitRect(GRECT* r);
	bool BulletHit(float* ix, float* iy);
	bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker);




private:
	float x,y; // position
	int facing; // way facing -1/1
	float aimangle,targetangle;
	float aimspeed,aimspeedvel; 
	float thinkcount;
	bool readytoshoot;
	float shoottimer;
	int clip;
	float reloadtime;
	LPDIRECT3DTEXTURE9 tarm, thead, tleg, tbody, that;
	CPlayers* targetplayer;
	bool WearingHat;

	sSound* s_bulletflesh1;
	sSound* s_bulletflesh2;
	sSound* s_bulletflesh3;

	sSound* s_taunt1;
	sSound* s_taunt2;
	sSound* s_taunt3;
	sSound* s_taunt4;

	sSound* s_die1;
	sSound* s_die2;

	sSound* s_hat;

	RECT rect_hat, rect_hit;
public:
	void Shoot(void);
};
