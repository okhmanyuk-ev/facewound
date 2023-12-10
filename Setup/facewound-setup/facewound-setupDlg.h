// facewound-setupDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <d3dx9.h>


// CfacewoundsetupDlg dialog
class CfacewoundsetupDlg : public CDialog
{
// Construction
public:
	CfacewoundsetupDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FACEWOUNDSETUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL bWindowed;
	BOOL bShaders;
	CString sDetailLevel;
	CComboBox comboDetail;
	BOOL bSoundEffects;
	CToolTipCtrl *m_ToolTip;
	CButton m_windowed;
	CButton m_shaders;
	CButton m_soundeffects;
	LPDIRECT3D9	m_pD3D;
	CListBox m_resolutions;
	CString sResolution;
};
