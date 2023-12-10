#pragma once

#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "landscape.h"

#define NCTYPE_PLAYER 0
#define NCTYPE_BULLETTIME 1
#define NCTYPE_PLAYERSHOOT 2

class CCore;

class cDemo
{
public:
	cDemo(void);
	~cDemo(void);

	CCore* pcore;

	bool bRecording;
	bool bPlaying;

	void Init();
	void Loop(void);
	void Record(char* Filename);
	void RecordEnd(void);
	void DoRecord(void);
	void DoPlay(void);
	void Play(char* filename);

	FILE* pFile;
	long int iTimer;
	bool ProcessCommand(char* command);
	void Stop(void);

	int Heartbeat;
	DWORD LastSnap;
	DWORD InternalTimer;

	void AddItem(int type, float arg1, float arg2, float arg3, float arg4, float arg5, float arg6, float arg7, float arg8, float arg9, float arg10, int playerid);
};
