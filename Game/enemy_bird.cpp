#include "enemy_bird.h"
#include "core.h"

enemy_bird::enemy_bird(void)
{
	m_x=0;
	m_y=0;


//	zombie.bmp

}

enemy_bird::~enemy_bird(void)
{
}

void enemy_bird::Do(void)
{
}

void enemy_bird::Draw(int layer)
{
}

bool enemy_bird::LoadStuff(void)
{
	return false;
}

void enemy_bird::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}
