#pragma once

class LevelEdit;
// cMapProperties dialog

class cMapProperties : public CDialog
{
	DECLARE_DYNAMIC(cMapProperties)

public:
	cMapProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~cMapProperties();

// Dialog Data
	enum { IDD = IDD_MAPPROPERTIES };

	LevelEdit* pLevEdit;
	void SetLeveditClass(LevelEdit* leved);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
//	DECLARE_DHTML_EVENT_MAP()
public:
	CString mapname;
	afx_msg void OnEnChangeEditAuthorname();
	CString authorname;
	CString authoremail;
	CString skyname;
	float skyspeed;
	float gravity;
	afx_msg void OnBnClickedOk();
};
