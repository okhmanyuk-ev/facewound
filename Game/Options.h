#pragma once
#include "cTextures.h"
#include "fmod.h"

#define NUM_KEYS 30

struct g_checkbox
{
	bool on;
	char name[100];
	float x, y;
};

struct g_slider
{
	float value;
	char name[100];
	float x, y;
	bool selected;
};


struct g_keybind
{
	char name[100];
	char command[100];
	char keyname[100];
	int keynum;
	float x, y;
	bool selected;

	g_keybind()
	{
		x=0;
		y=0;
		selected=false;
	}
};

class COptions
{
public:
	COptions(void);
	~COptions(void);

	CCore* pcore;
	void Draw(void);
	void Init(void);

	g_checkbox enablesound;
	g_slider soundvolume;
	g_slider musicvolume;

	g_keybind keybinds[NUM_KEYS];

	LPDIRECT3DTEXTURE9 t_opbits;
	void Do(void);
	int mouseover;
	RECT buttons[10];

	void OverButton(void);
	void DoMouseClick(void);
	void DrawSlider(g_slider* s);
	void SaveOptions(void);
	void DoKeyBinds(void);
	void DrawKeyBind(g_keybind* k);
	bool MouseDown;
	void LoadKeys(void);
	void AddKeyBind(char* name, char* command);
	void BindKeys(void);
	bool DoingKey;
};
