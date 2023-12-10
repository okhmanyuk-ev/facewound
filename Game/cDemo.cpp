#include "cDemo.h"
#include "core.h"


#define PCORE pcore
#define PLY pcore->players[0].player



cDemo::cDemo(void)
{
}

cDemo::~cDemo(void)
{
}


void cDemo::Init()
{
	bRecording = false;
	bPlaying = false;
	Heartbeat = 20;

}
void cDemo::Loop(void)
{
	InternalTimer = InternalTimer + pcore->gfx.tick;
	if (this->bRecording) this->DoRecord();
	if (this->bPlaying) this->DoPlay();

}

void cDemo::Record(char* Filename)
{
	if (this->bRecording) return;
	if (this->bPlaying) return;


	iTimer = 0;
	pFile = fopen(Filename, "wb");
	LastSnap = 0;
	InternalTimer = 0;



	this->pcore->gui.WriteToConsole(10,255,0,0,"Recording Started..");
	this->pcore->gui.ShowConsole = false;

	bRecording=true;
}

void cDemo::RecordEnd(void)
{
	if (!this->bRecording) return;

	this->pcore->gui.WriteToConsole(10,255,0,0,"Recording Stopped");

	bRecording=false;

	fclose(pFile);
}

void cDemo::DoRecord(void)
{
	if (LastSnap+Heartbeat < InternalTimer)
	{
		LastSnap = LastSnap + Heartbeat;
		iTimer++;
	}
	else
	{
		return;
	}

	long int timer;
	int argi;
	float argf[10];
	char buffer[500];
	int playerid=0;

	argi = NCTYPE_PLAYER;
	argf[0] = PLY.m_fX;
	argf[1] = PLY.m_fY;
	argf[2] = PLY.m_fXVel;
	argf[3] = PLY.m_fYVel;
	argf[4] = PLY.m_AimAngle;



	sprintf(buffer,"%i:%i;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%i\n",iTimer, argi, argf[0], argf[1], argf[2], argf[3], argf[4], argf[5], argf[6], argf[7], argf[8], argf[9], playerid);
	fputs(buffer,pFile);



}

void cDemo::DoPlay(void)
{
	if (feof(pFile))  {Stop();return;}

	if (LastSnap+Heartbeat < InternalTimer)
	{
		LastSnap = InternalTimer;
		iTimer++;
	}
	else
	{
		return;
	}

	char line[200];
	fgets(line,200*sizeof(char),pFile);
	while (ProcessCommand(line))
	{
		fgets(line,200*sizeof(char),pFile);
		if (feof(pFile)) {Stop();return;}
	}
}

void cDemo::Play(char* Filename)
{
	if (this->bRecording) return;
	if (this->bPlaying) return;

	this->pcore->gui.WriteToConsole(10,255,0,0,"Playing..");
	LastSnap = 0;
	InternalTimer = 0;

	pFile = fopen(Filename, "r");
	bPlaying=true;
}

bool cDemo::ProcessCommand(char* command)
{
	long int timer;
	int argi;
	float argf[10];
	int playerid;

	sscanf(command, "%i:%i;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%i", &timer, &argi, &argf[0], &argf[1], &argf[2], &argf[3], &argf[4], &argf[5], &argf[6], &argf[7], &argf[8], &argf[9], &playerid);

	if (argi==NCTYPE_PLAYER)
	{
		PLY.m_fX = argf[0];
		PLY.m_fY = argf[1];
		PLY.m_fXVel = argf[2];
		PLY.m_fYVel = argf[3];
		PLY.m_AimAngle = argf[4];
	}
	else if (argi==NCTYPE_PLAYERSHOOT)
	{
//		pcore->cdemo.AddItem(NCTYPE_PLAYERSHOOT,oldx,oldy,oldxvel, oldyvel, player->m_iWeapon, player->m_AimAngle,0,0,0,0);

		LPParticle lp = pcore->landscape.BulletShoot((int)argf[4],argf[0],argf[1],argf[5],1);
		if (lp!=NULL)
		{
			lp->pplayer = &PLY;
			lp->x = argf[0];
			lp->y = argf[1];
			lp->xvel = argf[2];
			lp->yvel = argf[3];
			PLY.m_AimAngle = argf[5];
			PLY.m_iWeapon = (int)argf[4];
		}
		

	}



	return false;
}

void cDemo::Stop(void)
{
	if (!this->bPlaying) return;

	this->pcore->gui.WriteToConsole(10,255,0,0,"End of demo");
	this->bPlaying = false;
	fclose(pFile);

}

void cDemo::AddItem(int type, float arg1, float arg2, float arg3, float arg4, float arg5, float arg6, float arg7, float arg8, float arg9, float arg10, int playerid)
{
	if (!this->bRecording) return;

	char buffer[200];

	sprintf(buffer,"%i:%i;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%.04f;%i\n",iTimer, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, playerid);
	fputs(buffer,pFile);
}
