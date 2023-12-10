#pragma once

#define FW_VERSION 0.07f

#define MAX_CLIENTS 128
#define DEFAULT_MOD_FOLDER "fwound"
#define CONFIG_FILENAME "settings/config.cfg"

#include <windows.h>
#include <winsock.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "skills.h"
#include "mapeditor/entity-defs.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "Landscape.h"
#include "Weapons.h"
#include "Players.h"
#include "Entitys.h"
#include "process.h"
#include "enemies.h"
//#include "netcode.h"
#include "cprops.h"
#include "GameIntro.h"
#include "GarrysSoundWrapper.h"
#include "Gui.h"
#include "cTextures.h"
#include "mainmenu.h"
#include "gameover.h"
#include "gameinput.h"
#include "options.h"
#include "EndLevel.h"
#include "cDemo.h"


#define EF_CONTRAST 0
#define EF_BLUR 1
#define EF_DOWN 2
#define EF_LEVELSPECIFIC 3



struct PPVERT
{
    float x, y, z, rhw;
    float tu, tv;       // Texcoord for post-process source
    const static D3DVERTEXELEMENT9 Decl[4];
};






inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

inline SQUARED(float val)
{
	return val*val;
}

struct splayers
{
	CPlayers player;
	bool inuse;
	unsigned int userid;

	// for multiplayer
	SOCKADDR_IN addr;
	int iaddrsize; 
	splayers()
	{
		inuse=false;
	}
};


struct SSettings
{
	bool camerascorch;
	bool showfps;
	bool repositionbuycursor;
	bool enablemusic;
	float musicvolume;
	int sb, db;
	unsigned int sleeptime;
	float BlurScale;
	bool BloomBlur;
	bool shaders;
	bool Bumping;


	bool SoundStretching;
	bool DynamicSoundStretching;

	bool FastSwitch;


	float fFlareScale;

	SSettings()
	{
		Bumping = true;
		camerascorch=false;
		showfps=false;
		repositionbuycursor=false;
		enablemusic=true;
		fFlareScale=1.0f;
		musicvolume=0.33f;
		sb = 5;
		db = 7;
		sleeptime=0;
		BloomBlur=true;
		BlurScale=1.0f;
		shaders=true;
		SoundStretching = true;
		DynamicSoundStretching = true;
		FastSwitch = true;
	}
};

struct SStats
{
	float Plutonium;
	float Enemies;
	float TimeTaken;
	int Lives;
	int ShotsFired;
	int ShotsLanded;
	int HiddenAreas;

	SStats()
	{
		Plutonium=0;
		Enemies=0;
		TimeTaken=0;
		Lives=3;
		HiddenAreas=0;
	}
};

class CCore
{

public:

	CCore(void);
	~CCore(void);

	SSettings Settings;

	char modfolder[100];

	bool Init(HINSTANCE hInstance, LPSTR lpCmdLine);
	void GameLoop(void);
	void LoadSettings(float* Width, float* Height, bool* Windowed);
	static LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	HWND hWnd;
	
	int fps, fpsdisplaymodel;
	float fpstimer;

	static bool draggingsize;

	float timemultiplier;
	float oldtimemultiplier;

	POINT point;
	float x, y;

	CGarrysGraphics gfx;
	CLandscape landscape;
	CWeapons weapons;
	Entitys ents;
	cProps props;
	cTextures textures;
	CGarrysSoundWrapper sound;
	CMainMenu menu;
	CGameOver gameover;
	static CGarrysGraphics* gfxptr;
	static CLandscape* landscapeptr;
	CEnemies enemies;
//	CNetcode netcode;
	CGui gui;
	splayers players[MAX_CLIENTS];
	CGameInput input;
	cDemo cdemo;

	float NetCode_PlayerMoveTimer;
	float m_Netcode_UpdateTimer;
	
	float XPosOnScreen(float xpos);
	float YPosOnScreen(float ypos);

//	HANDLE mutexHandle;

	int MyPlayerID;
	char player_Model[200];
	char player_Name[200];


	void Run(void);
	int AddPlayer(void);
	int AddPlayer(unsigned int id);
	float s_MouthOffDelay;
	bool bWeaponSelect;
	bool DontShootUntilFireHasBeenReleased;

	float AimLength;

	void DrawGameScreen(void);

	float Screen_a;
	float Screen_r;
	float Screen_g;
	float Screen_b;

	float Screen_colour_return_speed;
	float Screen_alpha_return_speed;

	bool m_FirstLoop;
	float gravity;
	void UpdateCursorPosition(void);
	void MoverMovePlayer(sProps* p, float x, float y);
	bool Trigger(char* name);
	void OutputTimer(char* teststring);
	clock_t timer_started;
	void StartTimer(void);
	unsigned int iGameState;
	void MainGameHub(void);
	CGameIntro gameintro;
	void StartGame(void);
	void toMenuScreen(void);
	bool inGame;
	void ResumeGame(void);
	void DoCommandLine(LPSTR lpCmdLine);
	bool bSkipMenus;
	char firstmap[50];
	bool b_LoadingMap;

	void GameOver(void);
	void ResetRoundStats(void);
	void ResetGameStats(void);

	bool bPaused;
	void GameInput(void);

	float ScreenZoom; // this is to make the zoom work regardless of the size of the screen
	void StartLevelEnd(char* nextlevel);

	bool doingLevelEnd;
	float doingLevelEndTimer;
	float LastPlut, LastEnemies;

	LPDIRECT3DTEXTURE9 t_color;
	char LevelEndNextLevel[100];

	sSound* s_Credit;

	void SetAlphaMode(bool on);
	bool m_CurrentBlendMode;

	SStats LevelStats;
	SStats GlobalStats;

	void StartLevel(void);
	void StartLevelLoop(void);
	float m_Timer;
	bool doStartLevel;
	
	void StartNextLevel(char* levelname);
	void StartLevelMusic(void);
	void FreeEverything(void);
	void ExitGame(void);
	void DoDieRoutine(void);
	void RestartAfterDead(void);

	

	sSound* s_3, *s_2, *s_1, *s_go;
	void DoAttack(void);
	bool bAttacked;
	bool bShowBuyMenu;

	float RecoilMultiplier;

	float TimerA, TimerAOld; // for sin functions that obey bullettime
	void RefreshMusicVolume(void);
	void Loading(int percent);
	LPDIRECT3DTEXTURE9 t_Loading;

	LPDIRECT3DTEXTURE9 RenderTarget;
	LPDIRECT3DTEXTURE9 RenderTargetTwo;
	//LPDIRECT3DTEXTURE9 RenderTargetSmall;
	LPDIRECT3DTEXTURE9 RenderTargetNormal;


	CEndLevel EndLevel;

	void PostProcessSmallTarget(void);
	void PostProcessFullScreen(void);
	
	void InitPostProcessing(void);
	LPD3DXEFFECT	Effect;
	LPD3DXEFFECT	GlobalEffect;

	bool bProcessingLevelStart;

	float screenmovex, screenmovey;

	void CreateRenderTargerts(void);

	float fx_RainDropTimer;
	void LostDevice(void);
	void ResetDevice(void);
	void CheckCapabilities(void);
	bool EscapeDown;

	float Width, Height;

	unsigned int ViewLayer;


	void PostProcessFullScreenFinal(void);
	void DrawNormalBuffer(void);
	void RenderWater(void);
};

