#pragma once

#include "LevelEdit.h"
// CEditTextures dialog

class CEditTextures : public CDialog
{
	DECLARE_DYNAMIC(CEditTextures)

public:
	CEditTextures(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTextures();
	LevelEdit* pLevEdit;
	void SetLeveditClass(LevelEdit* leved);

// Dialog Data
	enum { IDD = IDD_EDITTEXTUREPATHS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString t1;
	CString t2;
	CString t3;
	CString t4;
	CString t5;
	CString t6;
	CString t7;
	CString t8;
	CString t9;
	CString t10;
	afx_msg void OnBnClickedOk();
};
