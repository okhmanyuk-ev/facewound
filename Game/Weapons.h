
#ifndef _INCWEAPONS
#define _INCWEAPONS


#include "Globals.h"
#include "Landscape.h"
//#include "Players.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "GarrysSoundWrapper.h"



#define NUMWEAPONS 30

#define AMMO_RIFLE 0
#define AMMO_9MM 1
#define AMMO_SHOTGUN 2
#define AMMO_CHAIN 3
#define AMMO_ARROWS 4
#define AMMO_GRENADES 5
#define AMMO_LANDMINES 6
#define AMMO_PETROL 7
#define AMMO_MOLOTOV 8
#define AMMO_NONE 9

#define ST_AUTOMATIC 0
#define ST_PISTOL 1
#define ST_SHOTGUN 2

#define WEP_M16 0
#define WEP_BERETTA 1
#define WEP_SHOTGUN 2
#define WEP_MINIGUN 3
#define WEP_CROSSBOW 4
#define WEP_BOW 5
#define WEP_ARROWGUN 6
#define WEP_GRENADELAUNCHER 7
#define WEP_MP5SD 8
#define WEP_HANDGRENADE 9

#define WEP_GRENADELAUNCHERSTRESS 10

#define WEP_M60 11
#define WEP_LANDMINE 12

#define WEP_SMAW 14
#define WEP_AK47 15
#define WEP_MAC10 16
#define WEP_FLAMER 17

#define WEP_DEAGLE 18
#define WEP_PYTHON 19

#define WEP_MOLOTOV 20

#define WEP_CHAINSAW 21
#define WEP_BAT 22


// this class just draws the players. 
// Each player should have an instance of this class.

struct SGunAttributes
{
	char name[100];
	char poshname[100];
	float SpinupTime;
	float TimeBetweenShots;
	float BulletSpeed;
	float BulletSpeedRandomise;
	float MuzzleTip;
	float CarryOffset;
	float HeightOffset;
	unsigned int ClipSize;
	unsigned int ReloadSize;
	float ReloadTime;
	unsigned int AmmoType;
	unsigned int ShootType;
	float AimCone;
	unsigned int ReloadDropClip;
	float ScreenShake;

	float damage;
	float damagefade; // per tick
	unsigned int shelltype;
	unsigned int BulletParticle;
	sSound* ShootSound;
	sSound* ReloadSound;
	sSound* SpinSound;
	sSound* PumpSound;
	bool Pumper;
	bool Melee;
	int frames;
	float animspeed;
	float spawndelay;

	// for gui
	unsigned int slot;

	float Width, Height;

	SGunAttributes()
	{
		Melee=false;
		frames=1;
		CarryOffset = 0;
		HeightOffset = 0;
		Width=32;
		Height=128;
		BulletParticle = PARTICLE_BULLET;
		ShootSound = NULL;
		ReloadSound = NULL;
		SpinSound = NULL;
		ScreenShake=5;
		SpinupTime=0;
		animspeed=1.0f;
		spawndelay=0.0f;
	}
	

	unsigned int Shotgun_BuckshotNumber;  // only for shotguns
};

class CPlayers;
class CCore;

class CWeapons
{
	public:
		void Init(CGarrysGraphics *gfx);
		//CGarrysGraphics *m_pGfx;

		void Draw(unsigned int gunnum, CPlayers* player,float x, float y, float zoom);//, float radians, float radcx, float radcy, float scalex, float scaley);
		void DrawTracer(CPlayers* player, float a, float r, float g, float b);
		void DrawMuzzle(CPlayers* player);
		void DrawMuzzle(float x, float y, float aimangle, unsigned int weapon);

		LPDIRECT3DTEXTURE9 m_Texture[NUMWEAPONS];
		LPDIRECT3DTEXTURE9 m_GuiTexture[NUMWEAPONS];
	//	CGSound m_shootsound[NUMWEAPONS];
	//	CGSound m_reloadsound[NUMWEAPONS];
	//	CGSound m_reloadbulletsound[NUMWEAPONS];
		LPDIRECT3DTEXTURE9 m_TextureWhiteP;
		LPDIRECT3DTEXTURE9 m_TextureMuzzle;
		LPDIRECT3DTEXTURE9 m_TextureBright;

		CCore* pcore;

        SGunAttributes m_GunAttributes[NUMWEAPONS];
		void PlayShootSound(CPlayers* player);
		void PlayShootSound(float x, float y, unsigned int wep);
		void PlayPumpSound(float x, float y, unsigned int wep);
		void PlayReloadSound(CPlayers* player);
		void PlayReloadSound(float x, float y, unsigned int wep);
		void SpawnShell(CPlayers* player);
		int NameToID(char* wname);
		void LoadWeapon(unsigned int weapon_num, char* filename);
		void doRPGLaunchEffect(float x, float y, float angle);
		int LastShotChannel;

		sSound* ChainsawIdle;
		int ChainsawIdleChannel;
		void PlayIdleSound(int wep, float x, float y);
		void PlaySpinSound(int wep, float x, float y);
		void Draw(unsigned int gunnum, float x, float y, float zoom, float angle,  int direction, float scale, float shoottimer=0);
};

#endif