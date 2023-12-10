// editentity.cpp : implementation file
//

#include "stdafx.h"
#include "MapEditor.h"
#include "editentity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// editentity dialog


editentity::editentity(CWnd* pParent /*=NULL*/)
	: CDialog(editentity::IDD, pParent)
	, cs_label1(_T(""))
	, cs_label2(_T(""))
	, cs_label3(_T(""))
	, cs_label4(_T(""))
	, cs_label5(_T(""))
	, cs_label6(_T(""))
	, prop_name(_T(""))
{
	//{{AFX_DATA_INIT(editentity)
	m_xvalue = _T("");
	m_yvalue = _T("");
	m_typevalue = _T("");
	m_string1val = _T("");
	m_string2val = _T("");
	m_edita = _T("");
	m_editb = _T("");
	m_editc = _T("");
	m_editd = _T("");
	m_edite = _T("");
	m_editf = _T("");
	m_editg = _T("");
	m_edith = _T("");
	//}}AFX_DATA_INIT


}


void editentity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(editentity)
	DDX_Control(pDX, IDC_TYPECHOOSE, m_typechoose);
	DDX_Control(pDX, IDC_EDITY, m_y);
	DDX_Control(pDX, IDC_EDITX, m_x);
	DDX_Text(pDX, IDC_EDITX, m_xvalue);
	DDX_Text(pDX, IDC_EDITY, m_yvalue);
	DDX_CBString(pDX, IDC_TYPECHOOSE, m_typevalue);
	DDX_Text(pDX, IDC_STRING1, m_string1val);
	DDX_Text(pDX, IDC_STRING2, m_string2val);
	DDX_Text(pDX, IDC_EDIT2, m_edita);
	DDX_Text(pDX, IDC_EDIT3, m_editb);
	DDX_Text(pDX, IDC_EDIT4, m_editc);
	DDX_Text(pDX, IDC_EDIT5, m_editd);
	DDX_Text(pDX, IDC_EDIT6, m_edite);
	DDX_Text(pDX, IDC_EDIT7, m_editf);
	DDX_Text(pDX, IDC_EDIT8, m_editg);
	DDX_Text(pDX, IDC_EDIT9, m_edith);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LABEL1, cs_label1);
	DDX_Text(pDX, IDC_LABEL2, cs_label2);
	DDX_Text(pDX, IDC_LABEL3, cs_label3);
	DDX_Text(pDX, IDC_LABEL4, cs_label4);
	DDX_Text(pDX, IDC_LABEL5, cs_label5);
	DDX_Text(pDX, IDC_LABEL6, cs_label6);
	DDX_Text(pDX, IDC_STRING3, prop_name);
}


BEGIN_MESSAGE_MAP(editentity, CDialog)
	//{{AFX_MSG_MAP(editentity)
	ON_WM_MOUSEMOVE()
	ON_EN_CHANGE(IDC_EDITX, OnChangeX)
	ON_EN_CHANGE(IDC_EDITY, OnChangeY)
	ON_CBN_SELCHANGE(IDC_TYPECHOOSE, OnSelchangeTypechoose)
	ON_EN_CHANGE(IDC_STRING1, OnChangeString1)
	ON_EN_CHANGE(IDC_STRING2, OnChangeString2)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdita)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEditC)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEditD)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, OnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, OnChangeEdit9)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_STRING1, OnEnKillfocusString1)
	ON_STN_CLICKED(IDC_LABEL1, OnStnClickedLabel1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editentity message handlers

void editentity::SetLeveditClass(LevelEdit* leved, unsigned int tochange)
{
	pLevEdit = leved;
	entnumber = tochange;
}

void editentity::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateBackground();
	
	CDialog::OnMouseMove(nFlags, point);
}

void editentity::OnChangeX() 
{
	CString xv;
	m_x.GetWindowText(xv);
	pLevEdit->entitys[entnumber].x = atoi(xv);

	UpdateBackground();
}

void editentity::OnChangeY() 
{
	CString xv;
	m_y.GetWindowText(xv);
	pLevEdit->entitys[entnumber].y = atoi(xv);

	UpdateBackground();
}


void editentity::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	sprintf(pLevEdit->entitys[entnumber].stringa, m_string1val);
	pLevEdit->UpdateEntTexture(entnumber);

	sprintf(pLevEdit->entitys[entnumber].name,prop_name);

	CDialog::OnOK();
}

void editentity::UpdateBackground()
{
	CMapEditorApp* pointer;
	pointer = (CMapEditorApp*) AfxGetApp();
	pointer->OnIdle(0);
}


int editentity::DoModal() 
{
	//spawn;physics object;enemy_zombie;prop;consolecommand;enemy_soldierturret;ent_ladder;ent_pickup;ent_trigger;ent_elevator;ent_sound;ent_push;enemy_bonesy;enemy_rat;enemy_landmine;ent_levelend;ent_particlespawn;ent_lensflare;plutonium;ent_reflection;enemy_ghost;enemy_bat;enemy_statue;enemy_goth;enemy_sickzombie;enemy_pistolsoldier;ent_weather;enemy_m16soldier;enemy_smawsoldier;waterline;checkpoint;ent_rotatingprop;secretarea;ent_propparallax;enemy_jawfish;
	
	return CDialog::DoModal();
}

void editentity::OnSelchangeTypechoose() 
{
	UpdateData(TRUE);

	for (int i=0;i<300;i++)
	{
		if (strcmp(pLevEdit->entitytypes[i].name, m_typevalue)==0)
		{
			pLevEdit->entitys[entnumber].type=i;//m_typechoose.AddString(pLevEdit->entitytypes[i].name);
		}
	}

	this->DoLabels(pLevEdit->entitys[entnumber].type);
	UpdateData(false);

	pLevEdit->UpdateEntTexture(entnumber);
}

void editentity::OnChangeString1() 
{
	// null
}

void editentity::OnChangeString2() 
{
	UpdateData(TRUE);
	sprintf(pLevEdit->entitys[entnumber].stringb, m_string2val);
}

void editentity::OnChangeEdita() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].a = atof(m_edita);
}

void editentity::OnChangeEdit3() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].b = atof(m_editb);
}

void editentity::OnChangeEditC() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].c = atof(m_editc);
}

void editentity::OnChangeEditD() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].d = atof(m_editd);	
}

void editentity::OnChangeEdit6() //e
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].e = atof(m_edite);	
}

void editentity::OnChangeEdit7() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].f = atof(m_editf);	
}

void editentity::OnChangeEdit8() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].g = atof(m_editg);	
}

void editentity::OnChangeEdit9() 
{
	UpdateData(TRUE);
	pLevEdit->entitys[entnumber].h = atof(m_edith);	
}

void editentity::OnEnKillfocusString1()
{

}

void editentity::DoLabels(int id)
{

	m_typevalue = pLevEdit->entitytypes[pLevEdit->entitys[entnumber].type].name;

	cs_label1 = "not used";
	cs_label2 = "not used";
	cs_label3 = "not used";
	cs_label4 = "not used";
	cs_label5 = "not used";
	cs_label6 = "not used";

	// prop
	if (id==ENT_PROP)
	{
		cs_label1 = "Width /  Height";
		cs_label2 = "Breakable (0 for none)  / Explosion Size (0 for none)";
		cs_label3 = "SpawnParticle (0 for none)  / Particle # (0 for none)";
		cs_label5 = "texture filename ";
		cs_label4 = "1=additive / anim mode (1-12)";
	}

	if (id==ENT_LENSFLARE)
	{
		cs_label1 = "Width /  Height (important!)";
		cs_label2 = "Always On (0 for no)  / Size Multiplier (0 assumes 1)";
		cs_label5 = "texture filename ";
		cs_label4 = "blendmode (1=normal, 0 = additive) / anim mode (1-12)";
	}
	if (id==ENT_PLUTONIUM)
	{
		cs_label2 = "On (1 for no)/ none";
	}
	// console
	if (id==ENT_CONSOLE)
	{
		cs_label1 = "Width /  Height";
		cs_label2 = "Reactive time (0 for kill)  / not used";
		cs_label5 = "Console Command";
	}

	if (id==ENT_E_SOLTUR)
	{
		cs_label1 = "Way Facing (0=left, 1=right) / not used";
		cs_label5 = "Name";
	}
	if (id==ENT_LADDER)
	{
		cs_label1 = "width / height";
	}
	if (id==ENT_PICKUP) //prop
	{
		cs_label1 = "width / height";
		cs_label2 = "pickup num / notused";
		cs_label6 = "trigger on break";
	}
	if (id==ENT_TRIGGER)
	{
		cs_label1 = "width / height";
		cs_label2 = "time before activate again (in seconds) / null";
		cs_label6 = "object to trigger";
	}
	if(id==ENT_ELEVATOR)
	{
		cs_label1 = "width / height";
		cs_label2 = "distance up/down / speed";
		cs_label3 = "timer auto return / not used";
		cs_label5 = "texture name";
		cs_label6 = "trigger";
	}
	if(id==ENT_SOUND)
	{
		cs_label1 = "width / height";
		cs_label2 = "volume / z (center of head is -5.0)";
		cs_label2 = "-1 to loop forever / ";
		cs_label5 = "sound.wav (must be in the sounds/ folder)";
		cs_label6 = "trigger";
		cs_label4 = "not used / 1 = start on";
	}
	if(id==ENT_PUSH)
	{
		cs_label1 = "width / height";
		cs_label2 = "speed x / speed y";
		cs_label6 = "trigger";
	}
	if(id==ENT_E_BONESY)
	{
		cs_label6 = "trigger on die";
	}
	if(id==ENT_E_RAT)
	{
		cs_label6 = "trigger on die";
	}
	if (id==ENT_LEVELEND)
	{
		//cs_label1 = "width / height";
		//cs_label2 = "volume / z (center of head is -5.0)";
		//cs_label2 = "-1 to loop forever / ";
		cs_label5 = "Next level name";
		//cs_label6 = "trigger (not used yet)";
	}
	if (id==ENT_PARTICLESPAWNER)
	{
		cs_label1 = "width/height";
		cs_label2 = "particle # / delaybetweenspawn"; // c d
		cs_label3 = "delayrandom / firstdelay"; // ef
		cs_label4 = "# to spawn / 0=on;1=off"; // gh
	}
	if (id==ENT_REFLECTION)
	{
		cs_label1 = "Width /  Height";
		cs_label2 = "Layer (0 over background, 1 over everything)  / ";
		cs_label3 = "scale (def 32, up to 64)  / na";
		cs_label5 = "texture filename (eg 'reflections/1.bmp')";
		cs_label4 = "blendmode (0=normal, 1 = additive) / alpha (0 = completely transparent)";
	}
	if (id==ENT_WEATHER)
	{
		cs_label1 = "0: wind rain and mist";
	}
	if (id==ENT_WATERLINE)
	{
		cs_label1 = "y velocity";
	}
	if (id==ENT_ROTATINGPROP)
	{
		cs_label1 = "Width /  Height (needed to find the center!)";
		cs_label2 = "Start on (0=yes)  / Size Multiplier (0 assumes 1)";
		cs_label3 = "RotSpeed / ";
		cs_label5 = "texture filename ";
		cs_label4 = "blendmode (1=normal, 0 = additive) / anim mode (1-12)";
	}
	if (id==ENT_PARALLAXPROP)
	{
		cs_label1 = "Width /  Height (real!)";
		cs_label2 = "x multiplier  / y multiplier";
		cs_label3 = "layer / -)";
		cs_label5 = "texture filename ";
		cs_label4 = "1=additive / anim mode (1-12)";
	}
	
	


	//UpdateData(false);
}

void editentity::OnStnClickedLabel1()
{
	// TODO: Add your control notification handler code here
}

BOOL editentity::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_typechoose.AddString("test");
	m_typechoose.AddString(pLevEdit->entitytypes[ENT_SPAWN].name);

	for (int i=0;i<300;i++)
	{
		if (strcmp(pLevEdit->entitytypes[i].name,"")!=0)
		{
			m_typechoose.AddString(pLevEdit->entitytypes[i].name);
		}
	}

	ImportData();

	return true;
}

void editentity::ImportData(void)
{
	m_typevalue = pLevEdit->entitytypes[pLevEdit->entitys[entnumber].type].name;

	DoLabels(pLevEdit->entitys[entnumber].type);
	
	m_xvalue.Format("%i",(int)pLevEdit->entitys[entnumber].x);
	m_yvalue.Format("%i",(int)pLevEdit->entitys[entnumber].y);

	prop_name.Format("%s", pLevEdit->entitys[entnumber].name);

	m_string1val = pLevEdit->entitys[entnumber].stringa;
	m_string2val = pLevEdit->entitys[entnumber].stringb;

	m_edita.Format("%.4f",pLevEdit->entitys[entnumber].a);
	m_editb.Format("%.4f",pLevEdit->entitys[entnumber].b);
	m_editc.Format("%.4f",pLevEdit->entitys[entnumber].c);
	m_editd.Format("%.4f",pLevEdit->entitys[entnumber].d);
	m_edite.Format("%.4f",pLevEdit->entitys[entnumber].e);
	m_editf.Format("%.4f",pLevEdit->entitys[entnumber].f);
	m_editg.Format("%.4f",pLevEdit->entitys[entnumber].g);
	m_edith.Format("%.4f",pLevEdit->entitys[entnumber].h);

	UpdateData(false);
}
