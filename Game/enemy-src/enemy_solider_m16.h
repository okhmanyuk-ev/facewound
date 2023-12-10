#pragma once

#include "../enemies.h"
#include "../garryswrappers/GarrysGraphicsWrapper.h"
#include "../core.h"
#include "enemy_solider.h"

class enemy_soldier_m16 : public enemy_soldier
{
public:
	enemy_soldier_m16(void);
	~enemy_soldier_m16(void);

	void Draw(int layer);
	void HandleAttacking(void);
	void StartupVariables(void);
	
};
