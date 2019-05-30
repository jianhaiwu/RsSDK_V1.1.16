// hLiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "UpgradeDlg.h"


// CUpgradeDlg dialog

IMPLEMENT_DYNAMIC(CUpgradeDlg, CDialog)

CUpgradeDlg::CUpgradeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeDlg::IDD, pParent)
    , CQueryAndSetParam(CUpgradeDlg::IDD)
    , color_(RGB(rand()%256,rand()%256,rand()%256))
{

}

CUpgradeDlg::~CUpgradeDlg()
{
}

void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpgradeDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CUpgradeDlg message handlers

BOOL CUpgradeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUpgradeDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rc; 
    GetClientRect(rc); 
    pDC->FillSolidRect(rc,color_);
    return TRUE;
    return CDialog::OnEraseBkgnd(pDC);
}

BOOL CUpgradeDlg::Query() {
    return TRUE;
}

BOOL CUpgradeDlg::Set() {
    return FALSE;
}
