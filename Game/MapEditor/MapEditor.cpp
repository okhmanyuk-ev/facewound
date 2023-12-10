// MapEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MapEditor.h"

#include "MainFrm.h"
#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include "LevelEdit.h"
#include "editentity.h"
#include "edittextures.h"
#include "MapProperties.h"


#include "Windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD   d_threadID;
static DWORD WINAPI DrawThread( int i );

LevelEdit g_LevelEdit;


/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp

BEGIN_MESSAGE_MAP(CMapEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CMapEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEWDOC, OnFileNewdoc)
	ON_COMMAND(ID_TOOLBAREDITENTITY, OnToolbareditentity)
	ON_COMMAND(ID_PROPERTIES, OnToolbarMapProperties)
	ON_COMMAND(ID_MENU_TEXTURE, OnTextures)
	ON_COMMAND(ID_ADDENT, OnAddent)
	ON_COMMAND(ID_DELETEENT, OnDeleteent)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp construction

CMapEditorApp::CMapEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMapEditorApp object

CMapEditorApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp initialization

BOOL CMapEditorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMapEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMapEditorView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	g_LevelEdit.gfx.bSuspended = true;
	m_pMainWnd->UpdateWindow();
	g_LevelEdit.gfx.bSuspended = false;
	g_LevelEdit.Setup();
	//::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)DrawThread, NULL, 0, (LPDWORD)&d_threadID );


	return true;
}
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
		// No message handlers
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

// App command to run the dialog
void CMapEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


static DWORD WINAPI DrawThread( int i )
{
	while (true)
	{
	
	//	g_LevelEdit.Draw();

		Sleep(1000);


	}
	
	
	return 0;
}



void CMapEditorApp::OnFileNewdoc() 
{
	// TODO: Add your command handler code here
	g_LevelEdit.FileNew();
	
}

CDocument* CMapEditorApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	g_LevelEdit.landscape.LoadMap(lpszFileName);
	return CWinApp::OpenDocumentFile(lpszFileName);
}

void CMapEditorApp::SaveAs(LPCTSTR lpszFileName)
{
	g_LevelEdit.SaveMap(lpszFileName);
}

void CMapEditorApp::ResizeWindow()
{
	RECT winrect;
	GetClientRect(g_LevelEdit.hWnd,&winrect);

	//g_LevelEdit.gfx.bSuspended=true;
	g_LevelEdit.gfx.WindowResize((int)((winrect.right-winrect.left)-4<0?0:(winrect.right-winrect.left)-4) , 
								 (int)((winrect.bottom-winrect.top)-46<0?0:(winrect.bottom-winrect.top)-46));
	g_LevelEdit.landscape.SetWindowSize();

}

void CMapEditorApp::ReaquireDevices()
{
	g_LevelEdit.gfx.ReAquireDevices();	
	if (g_LevelEdit.gfx.bSuspended)
	{
		ResizeWindow();
		g_LevelEdit.gfx.bSuspended=false;
	}

}

BOOL CMapEditorApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	g_LevelEdit.Draw();

	return CWinApp::OnIdle(lCount);
}

void CMapEditorApp::NextTexture()
{
	
}

BOOL CMapEditorApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_CHAR  )
    {
		if (pMsg->wParam=='p')
		{
			g_LevelEdit.DrawTile++;
		}
		if (pMsg->wParam=='o')
		{
			g_LevelEdit.DrawTile--;
		}
		if (pMsg->wParam=='i')
		{
			g_LevelEdit.DrawTile=1;
		}
		if (pMsg->wParam==VK_DELETE)
		{
			g_LevelEdit.DelEntity();
		}

		if (g_LevelEdit.DrawTile<1) g_LevelEdit.DrawTile=1;

    } 
	else
	if ( pMsg->message == WM_KEYDOWN  )
    {
		if (pMsg->wParam==VK_DELETE)
		{
			g_LevelEdit.DelEntity();
		}
    } 
	
	return CWinApp::PreTranslateMessage(pMsg);
}

void CMapEditorApp::OnToolbareditentity() 
{
	// TODO: Add your command handler code here
	g_LevelEdit.AllowEdit=false;
	editentity editentity;
	editentity.SetLeveditClass(&g_LevelEdit,g_LevelEdit.SelectedEntity);
	editentity.DoModal();
	g_LevelEdit.AllowEdit=true;

}

void CMapEditorApp::OnToolbarMapProperties() 
{
	// TODO: Add your command handler code here
	g_LevelEdit.AllowEdit=false;
	cMapProperties mp;
	mp.SetLeveditClass(&g_LevelEdit);
	mp.DoModal();
	g_LevelEdit.AllowEdit=true;

}

void CMapEditorApp::OnTextures() 
{
	// TODO: Add your command handler code here


	TileSet* ts;
	if (g_LevelEdit.EditMode==1) ts = g_LevelEdit.landscape.textures;
	else if (g_LevelEdit.EditMode==2) ts = g_LevelEdit.landscape.texturesunder;
	else if (g_LevelEdit.EditMode==3) ts = g_LevelEdit.landscape.texturesover;
	else return;

	g_LevelEdit.AllowEdit=false;
	CEditTextures tex;

	tex.SetLeveditClass(&g_LevelEdit);
	tex.t1.Format("%s",ts[0].path);
	tex.t2.Format("%s",ts[1].path);
	tex.t3.Format("%s",ts[2].path);
	tex.t4.Format("%s",ts[3].path); 
	tex.t5.Format("%s",ts[4].path);
	tex.t6.Format("%s",ts[5].path);
	tex.t7.Format("%s",ts[6].path);
	tex.t8.Format("%s",ts[7].path);
	tex.t9.Format("%s",ts[8].path);
	tex.t10.Format("%s",ts[9].path);
	tex.DoModal();
	g_LevelEdit.AllowEdit=true;

}


void CMapEditorApp::OnAddent() 
{
	// TODO: Add your command handler code here
	g_LevelEdit.AddEntity();
}

void CMapEditorApp::OnDeleteent() 
{
	// TODO: Add your command handler code here
	g_LevelEdit.DelEntity();
}
