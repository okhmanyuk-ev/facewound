#pragma once

#include "enemies.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"

class enemy_bird : public enemy_base
{
public:
	enemy_bird(void);
	~enemy_bird(void);
	void Do(void);
	void Draw(int layer);
	float m_x;
	float m_y;

	LPDIRECT3DTEXTURE9 t_body;

	CEnemies* pCEnemies;
	bool LoadStuff(void);
	void SetXY(float x, float y);
};
