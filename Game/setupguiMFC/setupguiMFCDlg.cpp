// setupguiMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setupguiMFC.h"
#include "setupguiMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupguiMFCDlg dialog

CSetupguiMFCDlg::CSetupguiMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupguiMFCDlg::IDD, pParent)
	, m_PlayerName(_T(""))
	, m_PlayerModelString(_T(""))
	, DisableAll(FALSE)
	, SFX_MotionBlur(FALSE)
	, SFX_Bump(FALSE)
{
	//{{AFX_DATA_INIT(CSetupguiMFCDlg)
	Windowed = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetupguiMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupguiMFCDlg)
	DDX_Control(pDX, IDC_CHECK1, WindowedC);
	DDX_Control(pDX, IDOK, OkButton);
	DDX_Control(pDX, IDC_RESLIST, ListBox);
	DDX_Check(pDX, IDC_CHECK1, Windowed);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDITNAME, m_PlayerName);
	DDV_MaxChars(pDX, m_PlayerName, 20);
	DDX_Control(pDX, IDC_COMBO1, m_PlayerModelDropdown);
	DDX_CBString(pDX, IDC_COMBO1, m_PlayerModelString);
	DDX_Check(pDX, IDC_CHECK_DISABLESFX, DisableAll);
	DDX_Check(pDX, IDC_CHECK_SPECIALFX1, SFX_MotionBlur);
	DDX_Check(pDX, IDC_CHECK_SPECIALFX2, SFX_Bump);
	DDX_Control(pDX, IDC_CHECK_SPECIALFX2, SFX_BumpCtrl);
	DDX_Control(pDX, IDC_CHECK_SPECIALFX1, SFX_MotionBlurCtrl);
	DDX_Control(pDX, IDC_CHECK_DISABLESFX, DisableAllCtrl);
}

BEGIN_MESSAGE_MAP(CSetupguiMFCDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupguiMFCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SETFOCUS(IDC_RESLIST, OnSetfocusReslist)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_DISABLESFX, OnBnClickedCheckDisablesfx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupguiMFCDlg message handlers

BOOL CSetupguiMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TRACE("ADDING STRING");
	ListBox.AddString("800x600");
	ListBox.AddString("1024x768");
	ListBox.AddString("1280x1024");


	char filename[MAX_PATH];
	sprintf(filename, "players\\*.*");

	HANDLE lastdir;
	WIN32_FIND_DATA fdata;

	// list player models
	lastdir = FindFirstFile(filename, &fdata);

	if (lastdir!=INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(lastdir, &fdata))
		{
			if (strcmp(fdata.cFileName, ".")!=0 &&
				strcmp(fdata.cFileName, "..")!=0 &&
				(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				m_PlayerModelDropdown.AddString(fdata.cFileName);
			}
		}
	}
	else
	{
		MessageBox("Oops, I couldn't find the player models folder (players/)");
		m_PlayerModelDropdown.AddString("player");
	}

	m_PlayerName.Format("Name Goes Here!");
	UpdateData(false);
	m_PlayerModelDropdown.SelectString(0,"player");
	
	WindowedC.SetCheck(0);
	
	FILE* fp;
	int i=0;

	char string[500];

//	UpdateData(FALSE);
	fp = fopen("settings.txt","r");

	if (fp!=NULL)
	{
		char item[300];
		char value[300];

		while (fgets (string , 300 , fp)!=NULL)
		{
			sscanf(string,"%s %[^\n]",&item, &value);
		//	MessageBox(item);
		//	MessageBox(value);

			if (strcmp(item, "width")==0)
			{
				if (strcmp(value,"1024")==0) ListBox.SelectString(0,"1024x768");
				else if (strcmp(value,"1280")==0)ListBox.SelectString(0,"1280x1024");
				else ListBox.SelectString(0,"800x600");
				UpdateData(TRUE);
			}

			if (strcmp(item, "windowed")==0)
			{
				if (strcmp(value,"1")==0) WindowedC.SetCheck(1);
				UpdateData(TRUE);
			}

			if (strcmp(item, "name")==0)
			{
				m_PlayerName.Format(value);
				UpdateData(FALSE);
			}

			if (strcmp(item, "model")==0)
			{
				m_PlayerModelDropdown.SelectString(0,value);
				UpdateData(TRUE);
			}	
			if (strcmp(item, "sfx_disabled")==0)
			{
				if (strcmp(value,"1")==0)
				{
					this->EnableDisableSFX(false);
					DisableAllCtrl.SetCheck(1);
					UpdateData(TRUE);
				}

			}	
			if (strcmp(item, "sfx_motionblur")==0)
			{
				if (strcmp(value,"1")==0)
				{
					SFX_MotionBlurCtrl.SetCheck(1);
					UpdateData(TRUE);
				}

			}	
			if (strcmp(item, "sfx_bump")==0)
			{
				if (strcmp(value,"1")==0)
				{
					SFX_BumpCtrl.SetCheck(1);
					UpdateData(TRUE);
				}

			}	

		}
		

	}
	else
	{
		ListBox.SelectString(0,"800x600");
	}

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetupguiMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSetupguiMFCDlg::OnDestroy()
{
	//WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSetupguiMFCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSetupguiMFCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSetupguiMFCDlg::OnSetfocusReslist() 
{
	
}

//************************************************
//***************************************************
//*****************************************************
//******************************************************
void CSetupguiMFCDlg::OnOK() 
{
	CDialog::OnOK();

	UpdateData(TRUE);

	FILE* fp;
	char string[400];
	int i=0;

	fp = fopen("settings.txt","w");

	// RESOLUTION

	i = ListBox.GetCurSel();


	if (i==0 || i==LB_ERR)
		sprintf(string,"width 800\nheight 600\n",i);
	else if (i==1)
		sprintf(string,"width 1024\nheight 768\n",i);
	else if (i==2)
		sprintf(string,"width 1280\nheight 1024\n",i);
	fwrite(string,1,strlen(string),fp);

	// WINDOWED
	if (Windowed)
		sprintf(string,"windowed 1\n");
	else
		sprintf(string,"windowed 0\n");
	fwrite(string,1,strlen(string),fp);

	if (DisableAll)
		sprintf(string,"sfx_disabled 1\n");
	else
		sprintf(string,"sfx_disabled 0\n");
	fwrite(string,1,strlen(string),fp);

	if (SFX_Bump)
		sprintf(string,"sfx_bump 1\n");
	else
		sprintf(string,"sfx_bump 0\n");
	fwrite(string,1,strlen(string),fp);

	if (SFX_MotionBlur)
		sprintf(string,"sfx_motionblur 1\n");
	else
		sprintf(string,"sfx_motionblur 0\n");
	fwrite(string,1,strlen(string),fp);


	// write name
	if (strcmp(m_PlayerName,"")!=0)
	{
		sprintf(string,"name %s\n", m_PlayerName);
		fwrite(string,1,strlen(string),fp);
	}
	
	if (strcmp(m_PlayerModelString,"")!=0)
	{
		sprintf(string,"model %s\n", m_PlayerModelString);
		fwrite(string,1,strlen(string),fp);
	}

	sprintf(string,"\0");
	fwrite(string,1,strlen(string),fp);
	fclose(fp);

}

void CSetupguiMFCDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

void CSetupguiMFCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSetupguiMFCDlg::OnBnClickedCheckDisablesfx()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	EnableDisableSFX(!DisableAll);
	
}

void CSetupguiMFCDlg::EnableDisableSFX(bool enable)
{
	if (!enable)
	{
		SFX_BumpCtrl.EnableWindow(false);
		SFX_MotionBlurCtrl.EnableWindow(false);
	}
	else
	{
		SFX_BumpCtrl.EnableWindow(true);
		SFX_MotionBlurCtrl.EnableWindow(true);
	}
}
