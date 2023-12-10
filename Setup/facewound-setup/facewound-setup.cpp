// facewound-setup.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "facewound-setup.h"
#include "facewound-setupDlg.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfacewoundsetupApp

BEGIN_MESSAGE_MAP(CfacewoundsetupApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CfacewoundsetupApp construction

CfacewoundsetupApp::CfacewoundsetupApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CfacewoundsetupApp object

CfacewoundsetupApp theApp;


// CfacewoundsetupApp initialization
bool DirectoryExists(const char* FileName)
{
    struct stat my_stat;
    if (stat(FileName, &my_stat) != 0) return false;
    return ((my_stat.st_mode & S_IFDIR) != 0);
}

BOOL CfacewoundsetupApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();


	// make sure the settings fodler exists
	if (!DirectoryExists("settings"))
	{
		CreateDirectory("settings", NULL);
	}


	CWinApp::InitInstance();


	CfacewoundsetupDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
