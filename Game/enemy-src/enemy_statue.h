#pragma once

#include "../enemies.h"
#include "../garryswrappers/GarrysGraphicsWrapper.h"
#include "../core.h"

#define AISTATE_NOTACTIVE		0x000000001
#define AISTATE_WALKINGRIGHT	0x000000002
#define AISTATE_WALKINGLEFT		0x000000004
#define AISTATE_FALLING			0x000000008
#define AISTATE_STANDING		0x000000010
#define AISTATE_COMPLETINGTASK	0x000000020
#define AISTATE_ATTACKING		0x000000040

class enemy_statue : public enemy_base
{
public:
	enemy_statue(void);
	~enemy_statue(void);
	void Do(void);
	void Draw(int layer);
	float m_x, m_xvel;
	float m_nextdecision;
	float m_y, m_yvel;
	bool b_Activated;

	float aimangle;

	CPlayers* targetplayer;
	CEnemies* pCEnemies;
	bool LoadStuff(void);
	void SetXY(float x, float y);
	void HandleFalling(void);
	void HandleWalking(void);
	void DecideWhatToDo(void);
	bool BulletHit(float* x, float* y);
	bool BulletHitRect(GRECT* rectangle);
	void TakeDamage(float damage, float angle, float x, float y, bool Splash=false, LPParticle bullet=NULL);
	void DoFireDamage(float x, float y, float damage);
	void HandleAttacking(void);
	void InitAttack(void);
	void BulletHitSplash(float x, float y, float speed);
	bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker);

	float GetX();
	float GetY();
	float GetFacing();
	void AttachParticle(LPParticle particle);


	LPDIRECT3DTEXTURE9 t_body;
	LPDIRECT3DTEXTURE9 t_head;
	LPDIRECT3DTEXTURE9 t_corpse;

	void UpdateMyRect(void);
	//GRECT my_rect;

	float fighttimer;
	float animtimer;

	sSound* s_charge;
	sSound* s_shoot;
	

	
};
