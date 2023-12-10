// MapEditor.h : main header file for the MAPEDITOR application
//
#if !defined(AFX_MAPEDITOR_H__B02AB763_8416_48A5_A4AB_8011B04DBD4F__INCLUDED_)
#define AFX_MAPEDITOR_H__B02AB763_8416_48A5_A4AB_8011B04DBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp:
// See MapEditor.cpp for the implementation of this class
//

class CMapEditorApp : public CWinApp
{
public:
	void NextTexture();
	void ReaquireDevices ();
	void ResizeWindow();
	void SaveAs (LPCTSTR lpszFileName);
	CMapEditorApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMapEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNewdoc();
	afx_msg void OnToolbareditentity();
	afx_msg void OnToolbarMapProperties();
	afx_msg void OnTextures();
	afx_msg void OnAddent();
	afx_msg void OnDeleteent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITOR_H__B02AB763_8416_48A5_A4AB_8011B04DBD4F__INCLUDED_)
