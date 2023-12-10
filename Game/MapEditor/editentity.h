#if !defined(AFX_EDITENTITY_H__803B40E4_C288_4897_A693_A3D9B1264BD9__INCLUDED_)
#define AFX_EDITENTITY_H__803B40E4_C288_4897_A693_A3D9B1264BD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editentity.h : header file
//

#include "LevelEdit.h"

/////////////////////////////////////////////////////////////////////////////
// editentity dialog

class editentity : public CDialog
{
// Construction
public:
	void UpdateBackground();
	void SetLeveditClass(LevelEdit* leved, unsigned int tochange);
	editentity(CWnd* pParent = NULL);   // standard constructor
	LevelEdit* pLevEdit;
	unsigned int entnumber;

// Dialog Data
	//{{AFX_DATA(editentity)
	enum { IDD = IDD_EDITENTITY_DIALOG };
	CComboBox	m_typechoose;
	CEdit	m_y;
	CEdit	m_x;
	CString	m_xvalue;
	CString	m_yvalue;
	CString	m_typevalue;
	CString	m_string1val;
	CString	m_string2val;
	CString	m_edita;
	CString	m_editb;
	CString	m_editc;
	CString	m_editd;
	CString	m_edite;
	CString	m_editf;
	CString	m_editg;
	CString	m_edith;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editentity)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(editentity)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChangeX();
	virtual void OnOK();
	afx_msg void OnChangeY();
	afx_msg void OnSelchangeTypechoose();
	afx_msg void OnChangeString1();
	afx_msg void OnChangeString2();
	afx_msg void OnChangeEdita();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEditC();
	afx_msg void OnChangeEditD();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit7();
	afx_msg void OnChangeEdit8();
	afx_msg void OnChangeEdit9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusString1();
	CString cs_label1;
	void DoLabels(int id);
	CString cs_label2;
	CString cs_label3;
	CString cs_label4;
	CString cs_label5;
	CString cs_label6;
	afx_msg void OnStnClickedLabel1();
	CString prop_name;
	BOOL OnInitDialog(void);
	void ImportData(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITENTITY_H__803B40E4_C288_4897_A693_A3D9B1264BD9__INCLUDED_)
