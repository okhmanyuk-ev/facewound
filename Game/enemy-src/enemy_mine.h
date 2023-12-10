#pragma once

#include "../enemies.h"
#include "../garryswrappers/GarrysGraphicsWrapper.h"
#include "../core.h"

class enemy_mine : public enemy_base
{
public:
	enemy_mine(void);
	~enemy_mine(void);
	void Do(void);
	void Draw(int layer);
	float m_x;
	float m_nextdecision;
	float m_y, m_yvel;
	float m_facingscale;
	bool b_Activated;

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
	void HandleAttacking(void);
	void InitAttack(void);
	void BulletHitSplash(float x, float y, float speed);
	bool SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker);

	float GetX();
	float GetY();
	float GetFacing();
	void AttachParticle(LPParticle particle);
	float m_countdown;
	float m_initcountdown;


	LPDIRECT3DTEXTURE9 t_body;

	sSound* s_abouttoblow; 
	sSound* s_init;

	void UpdateMyRect(void);
	//GRECT my_rect;
	void doDetonate(void);
};
