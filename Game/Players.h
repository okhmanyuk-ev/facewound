#ifndef _INCPLAYERS
#define _INCPLAYERS


#include "Globals.h"
#include "Landscape.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "GarrysSoundWrapper.h"
#include "FlyingMachine.h" // plane
//#include "Weapons.h"

#define PLAYERSTATE_WALKING		0
#define PLAYERSTATE_FALLING		1
#define PLAYERSTATE_JUMPING		2

#define NUM_WEAPONS	30
#define NUM_TYPESOFAMMO 10
#define MAX_WEAPONS_HOLDING 100

// this class just draws the players. 
// Each player should have an instance of this class.

class CWeapons;
class CCore;

class CPlayers
{
	public:
		bool FlyingPlane;
		CFlyingMachine MyPlane;
		GRECT my_rect;
		int WeaponCarrying[MAX_WEAPONS_HOLDING];
		void UpdatePhysicalPresence(unsigned int num);
		bool bSpawned;
		bool bDuck;
		void Spawn(bool firstspawn=true);
		CPlayers();
		bool m_bEnabled;
		bool m_bShowHitboxes;
		unsigned int m_iState;
		unsigned int m_iWeapon;
		float m_SpinUpTimer;
		bool m_bMoving;
		float m_fWalkCount;
		long double m_fX;
		long double m_fY;
		long double m_fXVel;
		long double m_fYVel;
		float m_JumpStamina; // stop bunny hopping :)
		float m_JumpFPS;
		
		// the distance from x,y to the bullet hit

		float m_timeSinceShot; // time since last bullet shot

		int m_BulletsInClip[NUM_WEAPONS];
		float m_reloadTime;
		bool m_bReloading;
		int m_SpareAmmo[NUM_TYPESOFAMMO];

		POINT m_LastBulletHitPoint;
		float m_LastAimHitLength; 
		float m_LastAimAngle;
		float m_LastX;
		float m_LastY;
		float m_XShootShake;
		float m_YShootShake;
		float m_XBrightSparkScale;
		float m_YBrightSparkScale;

		unsigned int m_ShootCycle;
		float m_AimAngle;
		float m_AimHitLength; 
		POINT m_AimPoint;

		unsigned int m_Direction;
		LPDIRECT3DTEXTURE9 m_BodyTexture;
		LPDIRECT3DTEXTURE9 m_HeadTexture;
		LPDIRECT3DTEXTURE9 m_LegsTexture;
		LPDIRECT3DTEXTURE9 m_HitBoxTexture;
		LPDIRECT3DTEXTURE9 m_ArmTexture;

		LPDIRECT3DTEXTURE9 textures[10];
	//	CLandscape *m_landscape; // pointer to landscape - needed to get camera position
	//	CGarrysGraphics *m_pGfx;
	//	CWeapons *m_pWep;
		CCore* pcore;

		void CreatePlayer();

		void LoadBodyTexture(LPCTSTR filename);
		void LoadHeadTexture(LPCTSTR filename);
		void LoadLegsTexture(LPCTSTR filename);
		void LoadArmTexture(LPCTSTR filename);

		void Jump();
		void WalkLeft();
		void WalkRight();
		void Scale(float scale);

		void Do();

		void DrawShoot();
		void Shoot(); 


		float m_scale;

		float m_feetpos;
		float m_headpos;
		

//		dGeomID PhysShootRay;
//		dGeomID m_lastHitGeom;



		
		void SetXY(float x, float y);
		void Draw();

		float m_globalaplha;

		void Reload(void);
		void DoReload(void);
		void ChangeWeapon(int wepid);

		bool bIsPlayer;
		void Free(void);


		sSound* footstep1;
		sSound* footstep2;
		sSound* MouthOff[10];
		sSound* DeathSounds[10];
		sSound* PainSounds[10];
		sSound* s_ladder1;
		sSound* s_ladder2;

		void LoadPlayerModel(char* model);
		bool TakeDamage(float damage, CPlayers* player);
		float m_health;
		void SetPlayerName(char* name);

		char Player_Name[100];
		char Player_Model[100];

		unsigned int m_NumMouthOffs;
		unsigned int m_NumDeathSounds;
		unsigned int m_NumPainSounds;
		void DoMouthOff(void);
		void DoDeathSound(void);
		void DoPainSound(void);
		float m_LastMouthoff;

		float AimLength;
		void ClimbUpLadder(void);
		float m_fLadderStepCount;

		bool OnLadder;
		void ClimbDownLadder(void);
		bool FootCollision(bool fix, float x=-1, float y=-1);
		bool Collision(float x, float y);
		bool Collision(GRECT* grct);
		bool bFireDown;
		void ChangeModel(char* modelname);
		bool SplashDamage(float x, float y, float size, float damage, CPlayers* attacker);
		void Duck(void);
		void UnDuck(void);
		void Die(bool Gib);
		bool bDead;
		float dietimer;
		void DoPickups(void);
		bool GiveWeapon(unsigned int weapon);
		void GiveAmmo(unsigned int type, unsigned int amount);
		void StripWeaponsAndAmmo(void);
		int m_plutonium;
		void CollectPlutonium(int amount=1);
		bool SpendMoney(int amount);
		bool CanSpendMoney(int amount);
		bool Push(float x, float y);
		void UpdateMyRect(void);
		void DoMovements(void);
		void DoLadderStuff(void);
		bool TakeFireDamage(LPParticle p);
		void CountKill(void);
		bool bFired;
		bool bUnducking;
		float BulletTime;
		void DoBulletTimeToggle(void);
		void DoBulletTime(void);
		bool inBulletTime;
		void AddBulletTime(float amount);
		bool weapons_inPump;
		void ElectricDamage(void);
};

#endif