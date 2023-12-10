// MapProperties.cpp : implementation file
//

#include "stdafx.h"
#include "MapEditor.h"
#include "MapProperties.h"
#include "LevelEdit.h"

// cMapProperties dialog

IMPLEMENT_DYNAMIC(cMapProperties, CDialog)

cMapProperties::cMapProperties(CWnd* pParent /*=NULL*/) : CDialog(cMapProperties::IDD, pParent)
, mapname(_T(""))
, authorname(_T(""))
, authoremail(_T(""))
, skyname(_T(""))
, skyspeed(0)
, gravity(0)
{
}

cMapProperties::~cMapProperties()
{
}

void cMapProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAPNAME, mapname);
	DDX_Text(pDX, IDC_EDIT_AUTHORNAME, authorname);
	DDX_Text(pDX, IDC_EDIT_AUTHOREMAIL, authoremail);
	DDX_Text(pDX, IDC_EDIT_SKYNAME, skyname);
	DDX_Text(pDX, IDC_EDIT_SKYSPEED, skyspeed);
	DDX_Text(pDX, IDC_EDIT_GRAVITY, gravity);
}

BOOL cMapProperties::OnInitDialog()
{
//	CDHtmlDialog::OnInitDialog();

	this->mapname.Format(pLevEdit->landscape.m_MapProperties.maptitle);
	this->authorname.Format(pLevEdit->landscape.m_MapProperties.author);
	this->authoremail.Format(pLevEdit->landscape.m_MapProperties.music);
	this->skyname.Format(pLevEdit->landscape.m_MapProperties.skyname);
	this->gravity = pLevEdit->landscape.m_MapProperties.gravity;
	this->skyspeed = pLevEdit->landscape.m_MapProperties.skyspeed;

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(cMapProperties, CDialog)
	ON_EN_CHANGE(IDC_EDIT_AUTHORNAME, OnEnChangeEditAuthorname)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void cMapProperties::SetLeveditClass(LevelEdit* leved)
{
	pLevEdit = leved;
}
void cMapProperties::OnEnChangeEditAuthorname()
{
}

void cMapProperties::OnBnClickedOk()
{
	UpdateData(true);

	sprintf(pLevEdit->landscape.m_MapProperties.maptitle, mapname);
	sprintf(pLevEdit->landscape.m_MapProperties.author, authorname);
	sprintf(pLevEdit->landscape.m_MapProperties.music, authoremail);
	sprintf(pLevEdit->landscape.m_MapProperties.skyname, skyname);
	pLevEdit->landscape.m_MapProperties.gravity = this->gravity;
	pLevEdit->landscape.m_MapProperties.skyspeed = this->skyspeed;

	// TODO: Add your control notification handler code here
	OnOK();
}
