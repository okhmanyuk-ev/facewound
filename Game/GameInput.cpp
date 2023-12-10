#include "gameinput.h"
#include "core.h"

#define MOUSE1 1001
#define MOUSE2 1002
#define MOUSE3 1003
#define MOUSE4 1004

#define MOUSEWUP 1005
#define MOUSEWDOWN 1006

CGameInput::CGameInput(void)
{
	AddKey("mousewup", MOUSEWUP);
	AddKey("mousewdown", MOUSEWDOWN);

	AddKey("a", DIK_A);
	AddKey("b", DIK_B);
	AddKey("c", DIK_C);
	AddKey("d", DIK_D);
	AddKey("e", DIK_E);
	AddKey("f", DIK_F);
	AddKey("g", DIK_G);
	AddKey("h", DIK_H);
	AddKey("i", DIK_I);
	AddKey("j", DIK_J);
	AddKey("k", DIK_K);
	AddKey("l", DIK_L);
	AddKey("m", DIK_M);
	AddKey("n", DIK_N);
	AddKey("o", DIK_O);
	AddKey("p", DIK_P);
	AddKey("q", DIK_Q);
	AddKey("r", DIK_R);
	AddKey("s", DIK_S);
	AddKey("t", DIK_T);
	AddKey("u", DIK_U);
	AddKey("v", DIK_V);
	AddKey("w", DIK_W);
	AddKey("x", DIK_X);
	AddKey("y", DIK_Y);
	AddKey("z", DIK_Z);

	AddKey("1", DIK_1);
	AddKey("2", DIK_2);
	AddKey("3", DIK_3);
	AddKey("4", DIK_4);
	AddKey("5", DIK_5);
	AddKey("6", DIK_6);
	AddKey("7", DIK_7);
	AddKey("8", DIK_8);
	AddKey("9", DIK_9);
	AddKey("0", DIK_0);

	AddKey("space", DIK_SPACE);
	AddKey("lctrl", DIK_LCONTROL);
	AddKey("rctrl", DIK_RCONTROL);
	AddKey("rshift", DIK_RSHIFT);
	AddKey("lshift", DIK_RSHIFT);
	AddKey("ralt", DIK_RALT);
	AddKey("lalt", DIK_RALT);

	AddKey("f1", DIK_F1);
	AddKey("f2", DIK_F2);
	AddKey("f3", DIK_F3);
	AddKey("f4", DIK_F4);
	AddKey("f5", DIK_F5);
	AddKey("f6", DIK_F6);
	AddKey("f7", DIK_F7);
	AddKey("f8", DIK_F8);
	AddKey("f9", DIK_F9);
	AddKey("f10", DIK_F10);
	AddKey("f11", DIK_F11);
	AddKey("f12", DIK_F12);

	AddKey("insert", DIK_INSERT);
	AddKey("delete", DIK_DELETE);
	AddKey("home", DIK_HOME);
	AddKey("end", DIK_END);
	AddKey("pgdn", DIK_PGDN);
	AddKey("pgup", DIK_PGUP);

	AddKey("leftarrow", DIK_LEFT);
	AddKey("rightarrow", DIK_RIGHT);
	AddKey("uparrow", DIK_UP);
	AddKey("downarrow", DIK_DOWN);

	AddKey("kp_ins", DIK_NUMPAD0);
	AddKey("kp_del", DIK_NUMPADPERIOD);
	AddKey("kp_end", DIK_NUMPAD1);
	AddKey("kp_downarrow", DIK_NUMPAD2);
	AddKey("kp_pgdn", DIK_NUMPAD3);
	AddKey("kp_leftarrow", DIK_NUMPAD4);
	AddKey("kp_5", DIK_NUMPAD5);
	AddKey("kp_rightarrow", DIK_NUMPAD6);
	AddKey("kp_home", DIK_NUMPAD7);
	AddKey("kp_pgup", DIK_NUMPAD9);
	AddKey("kp_slash", DIK_NUMPADSLASH);
	AddKey("kp_minus", DIK_NUMPADMINUS);
	AddKey("kp_enter", DIK_NUMPADENTER);
	AddKey("*", DIK_NUMPADSTAR);

	AddKey(",", DIK_COMMA);
	AddKey(".", DIK_PERIOD);
	AddKey("/", DIK_SLASH);
	AddKey(";", DIK_SEMICOLON);
	AddKey("[", DIK_LBRACKET);
	AddKey("]", DIK_RBRACKET);
	AddKey("-", DIK_MINUS);
	AddKey("=", DIK_EQUALS);
	AddKey("~", DIK_NUMPADSTAR);

	AddKey("mouse1", MOUSE1);
	AddKey("mouse2", MOUSE2);
	AddKey("mouse3", MOUSE3);
	AddKey("mouse4", MOUSE4);

	AddKey("escape", DIK_ESCAPE);
}

CGameInput::~CGameInput(void)
{
}

bool CGameInput::bind(char* bindstring)
{
	char keyname[100], binding[100];
	sprintf(binding,"");
	sprintf(keyname,"");
	sscanf(bindstring,"%*s %s %[^\n]",keyname, binding);

	int keynum = KeyNameToID(keyname);


	if (keynum==0) 
	{
		pcore->gui.WriteToConsole(5,0,0,0,"Unknown keyname '%s'", keyname);
		return false;
	}

	// bind is null, they want to know the command
	if (binding[0]=='\0')
	{
		pcore->gui.WriteToConsole(5,0,0,0,"%s: %s",keyname, m_keys[keynum].action);
	}
	else
	if (binding[0]=='+')
	{
		m_keys[keynum].repeater = true;
		for (int i=1;i<strlen(binding);i++)
		{
			m_keys[keynum].action[i-1] = binding[i];
			m_keys[keynum].action[i] = '\0';
		}
		pcore->gui.WriteToConsole(5,0,0,0,"Bound %s to %s", binding, keyname);
	}
	else
	{
		if (strcmp(binding,"null")==0)
		{
			m_keys[keynum].repeater = false;
			sprintf(m_keys[keynum].action, "");	
		}
		else
		{
			m_keys[keynum].repeater = false;
			sprintf(m_keys[keynum].action, binding);
			pcore->gui.WriteToConsole(5,0,0,0,"Bound %s to %s", binding, keyname);
		}
	}

	

	return false;
}

unsigned int CGameInput::KeyNameToID(char* name)
{
	for (int i=1;i<NUM_GKEYS;i++)
	{
		if (strcmp(this->m_keys[i].keyname,name)==0)
		{
			return i;
		}
	}
	return 0;
}

void CGameInput::AddKey(char* name, unsigned int dik)
{
	for (int i=1;i<NUM_GKEYS;i++)
	{
		if (strcmp(this->m_keys[i].keyname,"")==0)
		{
			sprintf(m_keys[i].keyname,name);
			m_keys[i].keycode = dik;
			break;
		}
	}
}

void CGameInput::Do()
{
	for (int i=1;i<NUM_GKEYS;i++)
	{
		if (InputDown(m_keys[i].keycode))
		{
			if (!m_keys[i].down)
			{
				if (strlen(m_keys[i].action)>0)
				{
					pcore->gui.RunConsoleCommand(m_keys[i].action);
					if (!m_keys[i].repeater) m_keys[i].down = true;
				}
			}
		}
		else if (m_keys[i].down)
		{
			m_keys[i].down = false;
		}
	}
}

void CGameInput::SaveUserBinds(void)
{
	FILE* fp=NULL;

	fp = fopen(CONFIG_FILENAME, "w+");

	for (int i=1;i<NUM_GKEYS;i++)
	{
		if (strlen(m_keys[i].action)>0)
		{
			fputs("bind ",fp);
			fputs(m_keys[i].keyname,fp);
			fputs(" ",fp);
			if (m_keys[i].repeater) fputs("+",fp);
			fputs(m_keys[i].action,fp);
			fputs("\n",fp);
		}
	}

	fclose(fp);

}

bool CGameInput::InputDown(unsigned int key)
{
	if (key==0) return false;

	if (key<1000)
	{
		if (pcore->gfx.InputKeyDown(key)) return true;
	}
	else
	{
		if (key==MOUSE1 && pcore->gfx.m_bMouse1) return true;
		if (key==MOUSE2 && pcore->gfx.m_bMouse2) return true;
		if (key==MOUSE3 && pcore->gfx.m_bMouse3) return true;
		if (key==MOUSE4 && pcore->gfx.m_bMouse4) return true;
		if (key==MOUSEWUP && pcore->gfx.m_nMouseZ>0) return true;
		if (key==MOUSEWDOWN && pcore->gfx.m_nMouseZ<0) return true;
	}

	return false;
}


void CGameInput::BindCommandToKey(int key, char* command)
{
	char keyname[100];
	char bindstring[200];

	sprintf(keyname, IDtoKeyname(key));
	sprintf(bindstring, "bind %s %s",keyname, command);
	this->bind(bindstring);
}

char* CGameInput::IDtoKeyname(int id)
{
	for (int i=1;i<NUM_GKEYS;i++)
	{
		if (this->m_keys[i].keycode==id)
		{
			return m_keys[i].keyname;
		}
	}
	return NULL;
}
