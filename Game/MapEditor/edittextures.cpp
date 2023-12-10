// edittextures.cpp : implementation file
//

#include "stdafx.h"
#include "MapEditor.h"
#include "edittextures.h"


// CEditTextures dialog

IMPLEMENT_DYNAMIC(CEditTextures, CDialog)
CEditTextures::CEditTextures(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTextures::IDD, pParent)
	, t1(_T(""))
	, t2(_T(""))
{
}

CEditTextures::~CEditTextures()
{
}

void CEditTextures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_T1, t1);
	DDX_Text(pDX, IDC_EDIT_T2, t2);
	DDX_Text(pDX, IDC_EDIT_T3, t3);
	DDX_Text(pDX, IDC_EDIT_T4, t4);
	DDX_Text(pDX, IDC_EDIT_T5, t5);
	DDX_Text(pDX, IDC_EDIT_T6, t6);
	DDX_Text(pDX, IDC_EDIT_T7, t7);
	DDX_Text(pDX, IDC_EDIT_T8, t8);
	DDX_Text(pDX, IDC_EDIT_T9, t9);
	DDX_Text(pDX, IDC_EDIT_T10, t10);
}

void CEditTextures::SetLeveditClass(LevelEdit* leved)
{
	pLevEdit = leved;
}


BEGIN_MESSAGE_MAP(CEditTextures, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditTextures message handlers

void CEditTextures::OnBnClickedOk()
{
	UpdateData(TRUE);

	TileSet* ts;

	if (pLevEdit->EditMode==1) ts = pLevEdit->landscape.textures;
	if (pLevEdit->EditMode==2) ts = pLevEdit->landscape.texturesunder;
	if (pLevEdit->EditMode==3) ts = pLevEdit->landscape.texturesover;

	
	sprintf(ts[0].path,t1);
	sprintf(ts[1].path,t2);
	sprintf(ts[2].path,t3);
	sprintf(ts[3].path,t4);
	sprintf(ts[4].path,t5);
	sprintf(ts[5].path,t6);
	sprintf(ts[6].path,t7);
	sprintf(ts[7].path,t8);
	sprintf(ts[8].path,t9);
	sprintf(ts[9].path,t10);


	pLevEdit->landscape.LoadMapTextures(false);
	OnOK();
}
