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

class enemy_soldier : public enemy_base
{
public:
	enemy_soldier(void);
	~enemy_soldier(void);
	virtual void Do(void);
	virtual void Draw(int layer);
	float m_x, m_xvel;
	float m_nextdecision;
	float m_y, m_yvel;
	float m_facingscale;

	bool b_Activated;

	float aimangle;
	float targetangle;

	DWORD dwFlags;

	CPlayers* targetplayer;
	CEnemies* pCEnemies;
	virtual bool LoadStuff(void);
	virtual void SetXY(float x, float y);
	virtual void HandleFalling(void);
	virtual void HandleWalking(void);
	virtual void DecideWhatToDo(void);
	virtual bool BulletHit(float* x, float* y);
	virtual bool BulletHitRect(GRECT* rectangle);
	virtual void TakeDamage(float damage, float angle, float x, float y, bool Splash=false, LPParticle bullet=NULL);
	virtual void DoFireDamage(float x, float y, float damage);
	virtual void HandleAttacking(void);
	//virtual void InitAttack(void);
	virtual void BulletHitSplash(float x, float y, float speed);
	virtual bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker);

	float m_acceleration;
	float m_maxwalkingspeed;

	virtual float GetX();
	virtual float GetY();
	virtual float GetFacing();
	virtual void AttachParticle(LPParticle particle);


	LPDIRECT3DTEXTURE9 t_body;
	LPDIRECT3DTEXTURE9 t_head;
	LPDIRECT3DTEXTURE9 t_hat;
	LPDIRECT3DTEXTURE9 t_corpse;
	LPDIRECT3DTEXTURE9 t_arm;

	virtual void UpdateMyRect(void);
	virtual void StartupVariables(void);

	//GRECT my_rect;

	bool bFollowing;
	float fighttimer;
	float animtimer;

	sSound* s_hit1;
	sSound* s_hit2;
	sSound* s_hit3;
	sSound* s_hit4;
	sSound* s_die;


	int clip;
	int shootcount;

	float reloadtimer;

	bool helmet;


};
