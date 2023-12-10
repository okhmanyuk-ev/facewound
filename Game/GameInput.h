#pragma once
#include <stdio.h>
#define NUM_GKEYS 200

class CCore;

struct sKeyAction
{
	char keyname[20];
	unsigned int keycode;
	char action[100];
	bool down;
	bool repeater;

	sKeyAction()
	{
		sprintf(action, "");
		sprintf(keyname, "");
		keycode=0;	
		down=false;
		repeater=false;
	}


};

class CGameInput
{
public:
	CGameInput(void);
	~CGameInput(void);
	sKeyAction m_keys[NUM_GKEYS];
	bool bind(char* bindstring);
	CCore* pcore;
	unsigned int KeyNameToID(char* name);
	void AddKey(char* name, unsigned int dik);
	void Do();
	void SaveUserBinds(void);
	bool InputDown(unsigned int key);
	void BindCommandToKey(int key, char* command);
	char* IDtoKeyname(int id);
};
