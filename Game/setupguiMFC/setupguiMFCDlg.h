// setupguiMFCDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_SETUPGUIMFCDLG_H__CDB27BBE_9A94_4F16_9557_01C5FE2FF256__INCLUDED_)
#define AFX_SETUPGUIMFCDLG_H__CDB27BBE_9A94_4F16_9557_01C5FE2FF256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSetupguiMFCDlg dialog

class CSetupguiMFCDlg : public CDialog
{
// Construction
public:
	CSetupguiMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupguiMFCDlg)
	enum { IDD = IDD_SETUPGUIMFC_DIALOG };
	CButton	WindowedC;
	CButton	OkButton;
	CListBox	ListBox;
	BOOL	Windowed;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupguiMFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSetupguiMFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetfocusReslist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	CString m_PlayerName;
	CComboBox m_PlayerModelDropdown;
	CString m_PlayerModelString;
	BOOL DisableAll;
	BOOL SFX_MotionBlur;
	BOOL SFX_Bump;
	afx_msg void OnBnClickedCheckDisablesfx();
	CButton SFX_BumpCtrl;
	CButton SFX_MotionBlurCtrl;
	void EnableDisableSFX(bool enable);
	CButton DisableAllCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPGUIMFCDLG_H__CDB27BBE_9A94_4F16_9557_01C5FE2FF256__INCLUDED_)
