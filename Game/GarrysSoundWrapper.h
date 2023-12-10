#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dinput.h>



#include "fmod.h"

#define S_ZOMBIE_SIGHTED1		0
#define S_ZOMBIE_SIGHTED2		1
#define S_ZOMBIE_DEATH1			2
#define S_ZOMBIE_DEATH2			3
#define S_ZOMBIE_IDLE1			4
#define S_ZOMBIE_IDLE2			5
#define S_ZOMBIE_DIE			17

#define S_BULLETHIT_FLESH1		6
#define S_BULLETHIT_FLESH2		7
#define S_BULLETHIT_FLESH3		36

#define S_SHELLCASE_9MM1		8
#define S_SHELLCASE_9MM2		9
#define S_SHELLCASE_9MM3		10

#define S_SHELLCASE_SGUN1		11
#define S_SHELLCASE_SGUN2		12
#define S_SHELLCASE_SGUN3		13

#define S_SHELLCASE_RIF1		14
#define S_SHELLCASE_RIF2		15
#define S_SHELLCASE_RIF3		16

#define S_CHAINGUNCASE			18

#define S_BHIT_RICHOCET1		19
#define S_BHIT_RICHOCET2		20
#define S_BHIT_RICHOCET3		21
#define S_BHIT_RICHOCET4		22

#define S_BHIT_ARROW			23

#define S_SHOOT_M16				24
#define S_SHOOT_BERETTA			25
#define S_SHOOT_SHOTGUN			26
#define S_SHOOT_CROSSBOW		27
#define S_SHOOT_BOW				28
#define S_SHOOT_MINIGUN			29
#define S_SHOOT_GRENADELAUNCHER 37

#define S_RAINTAP				30

#define S_GUI_ROTATEWEAPON		31
#define S_GUI_OPENWEAPON		32
#define S_GUI_SELECTWEAPON		33

#define S_PLAYER_SPAWN			34

#define S_EXPLODE1				35

#define S_SHOOT_MP5SD			38

#define S_LADDER1				39
#define S_LADDER2				40

#define S_ZOMBIE_HEADBOUNCE		41
#define S_ZOMBIE_ARMBOUNCE1		42
#define S_ZOMBIE_ARMBOUNCE2		43

#define S_GRENADEBOUNCE			44

#define S_SOLDIERTURRET_TARG1	45
#define S_SOLDIERTURRET_TARG2	46
#define S_SOLDIERTURRET_TARG3	47
#define S_SOLDIERTURRET_TARG4	48

#define S_ClIPBOUNCE1			49
#define S_ClIPBOUNCE2			50

#define S_SHOOT_HANDGRENADE		51

#define S_RELOAD_M16				52
#define S_RELOAD_BERETTA			53
#define S_RELOAD_SHOTGUN			54
#define S_RELOAD_CROSSBOW			55
#define S_RELOAD_BOW				56
#define S_RELOAD_MINIGUN			57
#define S_RELOAD_GRENADELAUNCHER	58
//23

class CGarrysSoundWrapper;
class CCore;

struct sSound
{
	char fn[100];
	FSOUND_SAMPLE* samp;
	unsigned int priority;

	sSound* Next;
	sSound()
	{
		samp=NULL;
		Next=NULL;
	}

};

class CGarrysSoundWrapper
{
public:
	CGarrysSoundWrapper(void);
	~CGarrysSoundWrapper(void);
	void Init(void);

	HWND hWnd;

	float m_volume;

	void WriteToLog(char *lpszText, ...);
	void EnableLogging();
	bool b_EnableLogging;

	CCore* pcore;

	int PlaySnd(sSound * snd, float x, float y);
	int PlaySnd(sSound * snd, float x, float y, float pitchvariation, float z=0.0f, float timemultiplieroverride=0.0f, float volume=1.0f,  bool loop=false);

	bool soundenabled;

	sSound* FirstSound;

	sSound * LoadSound(unsigned int priority, char* Filename);
	int PlayFromFilename(char* filename, float x=0, float y=0, float z=0);
	unsigned int NumSoundsPlaying;
	void RefreshSound(void);
	FSOUND_STREAM * LoadStream(char * filename, bool loop);
	int PlayStream(FSOUND_STREAM* stream);
	void CloseStream(FSOUND_STREAM* stream);
	void FreeAll();
	bool IsPlaying(int channel);
	void StopSound(int channel);
	void UpdateSound(int channel, float x, float y);
	void UpdateFrequency(void);
	void SetChannelVolume(int channel, float volume);
	void PauseChannel(int channel);
	void ResumeChannel(int channel);
};






