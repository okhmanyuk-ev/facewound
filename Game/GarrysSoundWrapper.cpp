#include "garryssoundwrapper.h"
#include "core.h"

#define DEFAULT_LISTENER_DISTANCE -2.0f
#define XY_DISTANCE_DIVIDER 400.0f



CGarrysSoundWrapper::CGarrysSoundWrapper(void)
{
	NumSoundsPlaying = 0;
}

CGarrysSoundWrapper::~CGarrysSoundWrapper(void)
{
}

void CGarrysSoundWrapper::Init(void)
{
	WriteToLog("Init sound..<br>");
	m_volume = 1.0f;
	FirstSound = NULL;
	FSOUND_Init(22050  , 32, 0);
	FSOUND_SetDriver(0);   
	FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
	WriteToLog("Finsihed setting up Sound<br>");	
	soundenabled = true;
}


int CGarrysSoundWrapper::PlaySnd(sSound * snd, float x, float y)
{
	if (!soundenabled)return -1;
	return PlaySnd(snd, x, y, 0.0f);
}

int CGarrysSoundWrapper::PlaySnd(sSound * snd, float x, float y, float pitchvariation, float z, float timemultiplieroverride, float volume, bool loop)
{
	if (snd==NULL) return -1;
	if (snd->samp==NULL) return -1;
	if (!soundenabled)return -1;
	if (NumSoundsPlaying>10 && snd->priority<5) return -1;
	if (z==-5) z=DEFAULT_LISTENER_DISTANCE;

	NumSoundsPlaying++;

	float timemultiplier=pcore->timemultiplier;

	if (timemultiplieroverride!=0)
	{
		timemultiplier=timemultiplieroverride;
	}

	if (x!=0 && y!=0)
	{
		x = pcore->XPosOnScreen(x);
		y = pcore->YPosOnScreen(y);
		x = x/XY_DISTANCE_DIVIDER;
		y = y/XY_DISTANCE_DIVIDER;
	}
	
	float randnum = 1 - ((rand()%200/100.0f)-1)*pitchvariation;

	float listenerpos[3] = { 0.0f, -0.0f, DEFAULT_LISTENER_DISTANCE };
    float pos[3] = { x, y, z };
    float vel[3] = { 0,0,0 };

	int i;
	FSOUND_3D_Listener_SetAttributes(&listenerpos[0], &vel[0], 0, 0, 1.0f, 0, 1.0f, 0);
	i = FSOUND_PlaySoundEx(FSOUND_FREE, snd->samp, 0, TRUE);
	FSOUND_3D_SetAttributes(i, pos, vel);
    FSOUND_SetVolume(i, 255*m_volume);

	if (pcore->Settings.SoundStretching)
	{
		if (timemultiplier!=1 || pitchvariation!=0)
		FSOUND_SetFrequency(i, 22050*randnum*timemultiplier);
	}
	FSOUND_SetPaused(i, FALSE);

	return i;
}

void CGarrysSoundWrapper::EnableLogging()
{
	b_EnableLogging = true;
	
	FILE* pFile;
	pFile = fopen("logs/sound.html", "wb");
	if (pFile!=NULL) fclose(pFile);
}

void CGarrysSoundWrapper::WriteToLog(char *lpszText, ...)
{
	if(b_EnableLogging)
	{
		va_list argList;
		FILE *pFile;
		va_start(argList, lpszText);
		pFile = fopen("logs/sound.html", "a+");
		if (pFile!=NULL)
		{
			vfprintf(pFile, lpszText, argList);
			putc('\n', pFile);
			fclose(pFile);
		}
		va_end(argList);
	}
}

sSound * CGarrysSoundWrapper::LoadSound(unsigned int priority, char* inputname)
{
	FSOUND_SAMPLE* loadedsamp;
	sSound* p;
	
	char Filename[500];
	sprintf(Filename, "%s/%s", pcore->modfolder,inputname);

	// have we already loaded this sound
	p=FirstSound;
	while (p!=NULL)
	{
		if (strcmp(p->fn,inputname)==0)
		{
			//this->WriteToLog("Avoided loading sound twice [%s]<br>",Filename);
			return p;
		}
		p=p->Next;
	}

	if (!FileExists(Filename))
		sprintf(Filename, "%s/%s", DEFAULT_MOD_FOLDER,inputname);

	loadedsamp = FSOUND_Sample_Load(FSOUND_FREE, Filename, FSOUND_HW3D | FSOUND_LOOP_OFF, 0, 0);

	if (loadedsamp==NULL)
	{
		char errormsg[200];
 		sprintf(errormsg," Couldn't load sound:\n  '%s' ",inputname);
		MessageBox(NULL,errormsg,"Error",0);
		exit(1);
	}

	sSound* NewSound = new sSound();
	
	if (FirstSound==NULL) 
	{
		FirstSound = NewSound;
	}
	else 
	{
		p=FirstSound;
		while (p->Next!=NULL)
		{
			p=p->Next;
		}
		p->Next = NewSound;
	}

	NewSound->priority = priority;
	NewSound->samp = loadedsamp;
	sprintf(NewSound->fn,inputname);

	return NewSound;
}

int CGarrysSoundWrapper::PlayFromFilename(char* filename, float x, float y, float z)
{
	return this->PlaySnd(LoadSound(5,filename),x,y,0.0f,z);
}

void CGarrysSoundWrapper::RefreshSound(void)
{
	NumSoundsPlaying=0;
}

FSOUND_STREAM * CGarrysSoundWrapper::LoadStream(char * filename, bool loop)
{
	FSOUND_STREAM *stream;
	char Filename[500];
	sprintf(Filename, "%s/%s", pcore->modfolder,filename);
	if (!FileExists(Filename))
		sprintf(Filename, "%s/%s", DEFAULT_MOD_FOLDER,filename);


	stream = FSOUND_Stream_Open(Filename, (loop?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF),0,0);

	if (stream==NULL)
	{
		WriteToLog("Couldn't open stream: '%s'<br>",filename);
	}

	return stream;
}

int CGarrysSoundWrapper::PlayStream(FSOUND_STREAM* stream)
{
	if (stream==NULL)
	{
		WriteToLog("Attempted to play NULL stream!<br>");
		return -1;
	}

	return FSOUND_Stream_Play(FSOUND_FREE, stream);
}

void CGarrysSoundWrapper::CloseStream(FSOUND_STREAM* stream)
{
	FSOUND_Stream_Stop(stream);
	FSOUND_Stream_Close(stream);
}

void CGarrysSoundWrapper::FreeAll()
{
	sSound* p;
	p=FirstSound;
	while (p!=NULL)
	{
		//free
		p=p->Next;
	}
}

bool CGarrysSoundWrapper::IsPlaying(int channel)
{
	if (FSOUND_IsPlaying(channel)) return true;

	return false;
}

void CGarrysSoundWrapper::StopSound(int channel)
{
	FSOUND_StopSound(channel);
}

void CGarrysSoundWrapper::UpdateSound(int channel, float x, float y)
{
	if (x!=0 && y!=0)
	{
		x = pcore->XPosOnScreen(x);
		y = pcore->YPosOnScreen(y);
		x = x/XY_DISTANCE_DIVIDER;
		y = y/XY_DISTANCE_DIVIDER;
	}

	float pos[3] = {x,y,0};
    float vel[3] = {0,0,0};

	FSOUND_3D_GetAttributes(channel, pos, vel);
	float listenerpos[3] = { 0.0f, -0.0f, DEFAULT_LISTENER_DISTANCE };

	pos[0] = x;
	pos[1] = y;

	FSOUND_3D_Listener_SetAttributes(&listenerpos[0], &vel[0], 0, 0, 1.0f, 0, 1.0f, 0);
	FSOUND_3D_SetAttributes(channel, pos, vel);
}

void CGarrysSoundWrapper::UpdateFrequency(void)
{
	if (!pcore->Settings.SoundStretching) return;
	if (!pcore->Settings.DynamicSoundStretching) return;
	if (pcore->doStartLevel) return;

	for (int i=0; i<FSOUND_GetMaxChannels();i++)
	{
		if (this->IsPlaying(i))
		{
			FSOUND_SetFrequency(i, 22050*pcore->timemultiplier);
		}
	}
}

void CGarrysSoundWrapper::SetChannelVolume(int channel, float volume)
{
	FSOUND_SetVolume(channel, 255*volume);
}

void CGarrysSoundWrapper::PauseChannel(int channel)
{
	FSOUND_SetPaused(channel, TRUE);
}

void CGarrysSoundWrapper::ResumeChannel(int channel)
{
	FSOUND_SetPaused(channel, FALSE);
}
