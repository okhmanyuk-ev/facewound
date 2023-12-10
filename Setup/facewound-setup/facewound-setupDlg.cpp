// facewound-setupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "facewound-setup.h"
#include "facewound-setupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DETAIL_HIGH "Highest (best)"
#define DETAIL_LOW "Low"
#define DETAIL_LOWEST "Lowest (rubbish)"





// CfacewoundsetupDlg dialog



CfacewoundsetupDlg::CfacewoundsetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfacewoundsetupDlg::IDD, pParent)
	, bWindowed(FALSE)
	, bShaders(FALSE)
	, sDetailLevel(_T(""))
	, bSoundEffects(FALSE)
	, sResolution(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CfacewoundsetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK2, bWindowed);
	DDX_Check(pDX, IDC_CHECK1, bShaders);
	DDX_CBString(pDX, IDC_COMBO1, sDetailLevel);
	DDX_Control(pDX, IDC_COMBO1, comboDetail);
	DDX_Check(pDX, IDC_CHECK3, bSoundEffects);
	DDX_Control(pDX, IDC_CHECK2, m_windowed);
	DDX_Control(pDX, IDC_CHECK1, m_shaders);
	DDX_Control(pDX, IDC_CHECK3, m_soundeffects);
	DDX_Control(pDX, IDC_LIST1, m_resolutions);
	DDX_LBString(pDX, IDC_LIST1, sResolution);
}

BEGIN_MESSAGE_MAP(CfacewoundsetupDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CfacewoundsetupDlg::PreTranslateMessage(MSG* pMsg)
{
   if (NULL != m_ToolTip)
         m_ToolTip->RelayEvent(pMsg);

        return CDialog::PreTranslateMessage(pMsg);

}

// CfacewoundsetupDlg message handlers
BOOL CfacewoundsetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// enumerate display modes
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if (m_pD3D==NULL) 
	{ 
		::MessageBox(NULL,"Couldn't initialize Direct3D! Please make sure you have at least DirectX 9.0c installed!","error",MB_ICONEXCLAMATION); 
		exit(1);
	}

	// current desktop colour
	D3DDISPLAYMODE mode;
	D3DDISPLAYMODE retmode;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode);


	unsigned int NumModes = m_pD3D->GetAdapterModeCount(0,mode.Format);
	HRESULT hr=NULL;

	CString formatstring;

	int lastheight=0, lastwidth=0;

	for (unsigned int i=0;i<NumModes;i++)
	{
		hr = m_pD3D->EnumAdapterModes(0,mode.Format,i, &retmode);
		if (hr==S_OK)
		{
			if ((lastwidth != retmode.Width || lastheight != retmode.Height) && (retmode.Width>=640))
			{
				formatstring.Format("%i x %i",retmode.Width,retmode.Height);
				m_resolutions.AddString(formatstring);
				lastwidth = retmode.Width;
				lastheight = retmode.Height;
			}
		}
	}

	m_pD3D->Release();
	m_pD3D=NULL;

	m_resolutions.SelectString(0,"1024 x 768");

	

	comboDetail.AddString(DETAIL_HIGH);
	comboDetail.AddString(DETAIL_LOW);
	comboDetail.AddString(DETAIL_LOWEST);

	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	m_ToolTip->SetMaxTipWidth(300);
	m_ToolTip->AddTool(&m_windowed,"If you select this box the game will run in a window. The game will perform better if this is disabled.");
	m_ToolTip->AddTool(&m_shaders,"Selecting this will give you awesome graphics. You need a pretty modern graphics card to get this working. This is automatically disabled in-game if your graphics card can't do pixel shader 2.0");
	m_ToolTip->AddTool(&m_soundeffects,"The game sounds like ass if this is disabled. Only disable it if your sound sometimes cuts out randomly.");
	m_ToolTip->AddTool(&comboDetail,"Modern computers should be able to run on high - if your computer is shit lowering this will make the game run smoother.");
	m_ToolTip->Activate(true);

	sDetailLevel = DETAIL_HIGH;
	bShaders = true;
	bWindowed = false;
	bSoundEffects = true;


	// Load the settings
	FILE* fp;
	char string[500];
	fp = fopen("settings/system.cfg","r");

	unsigned int width, height;

	if (fp!=NULL)
	{
		char item[300];
		char value[300];

		while (fgets (string , 300 , fp)!=NULL)
		{
			sscanf(string,"%s %[^\n]",&item, &value);

			if (strcmp(item, "windowed")==0)
			{
				if (atoi(value)==1) bWindowed = true;
			}

			if (strcmp(item, "soundstretching")==0)
			{
				if (atoi(value)==0) bSoundEffects = false;
			}

			if (strcmp(item, "width")==0)
			{
				width = atoi(value);
			}
			
			if (strcmp(item, "height")==0)
			{
				height = atoi(value);
			}

			if (strcmp(item, "shadersoff")==0)
			{
				bShaders = false;
			}


			if (strcmp(item, "shaderson")==0)
			{
				bShaders = true;
			}

			if (strcmp(item, "detail")==0)
			{
				if (atoi(value)==0) sDetailLevel = DETAIL_LOWEST;
				if (atoi(value)==1) sDetailLevel = DETAIL_LOW;
				if (atoi(value)==2) sDetailLevel = DETAIL_HIGH;
			}
		}

	}

	CString resolutionstring;

	resolutionstring.Format("%i x %i", width, height);
	m_resolutions.SelectString(0,resolutionstring);

	UpdateData(false);






	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CfacewoundsetupDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CfacewoundsetupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfacewoundsetupDlg::OnBnClickedOk()
{
	UpdateData(true);

	FILE* fp=NULL;
	fp = fopen ("settings/system.cfg","w");
	char string[255];

	if (fp!=NULL)
	{
		int width=1024, height=768;

		sscanf(sResolution, "%i x %i",&width, &height);
		
		sprintf(string,"width %i",width);
		fputs(string,fp);

		fputs("\n",fp);

		sprintf(string,"height %i",height);
		fputs(string,fp);

		fputs("\n",fp);

		if (bWindowed) fputs("windowed 1",fp);
		else fputs("windowed 0",fp);

		fputs("\n",fp);

		if (bShaders) fputs("shaderson",fp);
		else fputs("shadersoff",fp);

		fputs("\n",fp);

		if (sDetailLevel == DETAIL_LOWEST) fputs("detail 0",fp);
		if (sDetailLevel == DETAIL_LOW) fputs("detail 1",fp);
		if (sDetailLevel == DETAIL_HIGH) fputs("detail 2",fp);

		fputs("\n",fp);

		if (bSoundEffects) fputs("soundstretching 1",fp);
		else fputs("soundstretching 0",fp);


		
		fclose(fp);
	}

	OnOK();
}
