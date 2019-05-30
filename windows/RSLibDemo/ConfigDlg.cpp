// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "ConfigDlg.h"
#include "RSLibDemoDlg.h"


// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
    , parent_(NULL)
    , init_over_(FALSE)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_QUERY, &CConfigDlg::OnBnClickedBtnQuery)
    ON_BN_CLICKED(IDC_BTN_SET, &CConfigDlg::OnBnClickedBtnSet)
    ON_BN_CLICKED(IDC_BTN_LIVE_CHANNEL, &CConfigDlg::OnBnClickedBtnLiveChannel)
    ON_BN_CLICKED(IDC_BTNUPGRADE, &CConfigDlg::OnBnClickedBtnupgrade)
	ON_BN_CLICKED(IDC_BTN_USER, &CConfigDlg::OnBnClickedBtnUser)
	ON_BN_CLICKED(IDC_BTN_HDD, &CConfigDlg::OnBnClickedBtnHdd)
    ON_BN_CLICKED(IDC_BTN_JSON, &CConfigDlg::OnBnClickedBtnJson)
END_MESSAGE_MAP()


// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    if (CChLiveDlg_.GetSafeHwnd() == NULL) {
        CChLiveDlg_.Create(CChLiveDlg_.IDD, GetDlgItem(IDC_STATIC_CONFIG));
        CChLiveDlg_.ShowWindow(SW_HIDE);
        vec_dialog_.push_back(&CChLiveDlg_);
    }

    if (CUpgradeDlg_.GetSafeHwnd() == NULL) {
        CUpgradeDlg_.Create(CUpgradeDlg_.IDD, GetDlgItem(IDC_STATIC_CONFIG));
        CUpgradeDlg_.ShowWindow(SW_HIDE);
        vec_dialog_.push_back(&CUpgradeDlg_);
    }

	if (CUserDlg_.GetSafeHwnd() == NULL) {
		CUserDlg_.Create(CUserDlg_.IDD, GetDlgItem(IDC_STATIC_CONFIG));
		CUserDlg_.ShowWindow(SW_HIDE);
		vec_dialog_.push_back(&CUserDlg_);
	}

	if(CHddDlg_.GetSafeHwnd() == NULL) {
		CHddDlg_.Create(CHddDlg_.IDD, GetDlgItem(IDC_STATIC_CONFIG));
		CHddDlg_.ShowWindow(SW_HIDE);
		vec_dialog_.push_back(&CHddDlg_);
	}

    if(CJsonPageDlg_.GetSafeHwnd() == NULL) {
        CJsonPageDlg_.Create(CJsonPageDlg_.IDD, GetDlgItem(IDC_STATIC_CONFIG));
        CJsonPageDlg_.ShowWindow(SW_HIDE);
        vec_dialog_.push_back(&CJsonPageDlg_);
    }

    vec_dialog_.at(0)->ShowWindow(SW_SHOW);
    init_over_ = TRUE;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if (init_over_) {
        CRect rect;
        GetDlgItem(IDC_STATIC_CONFIG)->GetClientRect(&rect);

        for (size_t i = 0; i < vec_dialog_.size(); i++) {
            vec_dialog_.at(i)->MoveWindow(&rect);
        }
    }
}

void CConfigDlg::SetParentWindow(CRSLibDemoDlg* parent) {
    parent_ = parent;
}

void CConfigDlg::OnBnClickedBtnQuery()
{
    // TODO: Add your control notification handler code here
    device_id dev = 0;
    DEVICE_INFO* channel_info = NULL;
    HTREEITEM item = g_device_tree_->GetSelectedItem();
    if (item && (channel_info = (DEVICE_INFO*)(g_device_tree_->GetItemData(item))) && channel_info->type == TYPE_CHANNEL) {
        dev = channel_info->device_info->device_session;
    } else if (item && (channel_info = (DEVICE_INFO*)(g_device_tree_->GetItemData(item))) && channel_info->type == TYPE_DEVICE) {
        dev = channel_info->device_session;
    }
    if (dev == 0) {
        MessageBox(_T("Please select at one device"), NULL, MB_OK | MB_ICONERROR);
        return;
    }

    for (size_t i = 0; i < vec_dialog_.size(); i++) {
        if (vec_dialog_.at(i)->IsWindowVisible()) {
            CQueryAndSetParam* base = dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i));
            base->dev_ = dev;
            base->value_.clear();

            if (base->Query()) {
                MessageBox(_T("Query Success"), NULL, MB_OK | MB_ICONINFORMATION);
            } else {
                MessageBox(_T("Query Failed"), NULL, MB_OK | MB_ICONERROR);
            }
            break;
        }
    }
}

void CConfigDlg::OnBnClickedBtnSet()
{
    // TODO: Add your control notification handler code here
    device_id dev = 0;
    DEVICE_INFO* channel_info = NULL;
    HTREEITEM item = g_device_tree_->GetSelectedItem();
    if (item && (channel_info = (DEVICE_INFO*)(g_device_tree_->GetItemData(item))) && channel_info->type == TYPE_CHANNEL) {
        dev = channel_info->device_info->device_session;
    } else if (item && (channel_info = (DEVICE_INFO*)(g_device_tree_->GetItemData(item))) && channel_info->type == TYPE_DEVICE) {
        dev = channel_info->device_session;
    }
    if (dev == 0) {
        MessageBox(_T("Please select at one device"), NULL, MB_OK | MB_ICONERROR);
        return;
    }

    for (size_t i = 0; i < vec_dialog_.size(); i++) {
        if (vec_dialog_.at(i)->IsWindowVisible()) {
            CQueryAndSetParam* base = dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i));
            base->dev_ = dev;
            if (base->Set()) {
                MessageBox(_T("Set Success"), NULL, MB_OK | MB_ICONINFORMATION);
            } else {
                MessageBox(_T("Set Failed"), NULL, MB_OK | MB_ICONERROR);
            }
            break;
        }
    }
}

void CConfigDlg::OnBnClickedBtnLiveChannel()
{
    // TODO: Add your control notification handler code here
    for (size_t i = 0; i < vec_dialog_.size(); i++) {
        if (dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i))->IDD == IDD_DLG_CHLIVE) {
            vec_dialog_.at(i)->ShowWindow(SW_SHOW);
        } else {
            vec_dialog_.at(i)->ShowWindow(SW_HIDE);
        }
    }
}

void CConfigDlg::OnBnClickedBtnupgrade()
{
    // TODO: Add your control notification handler code here
    for (size_t i = 0; i < vec_dialog_.size(); i++) {
        if (dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i))->IDD == IDD_DLG_UPGRADE) {
            vec_dialog_.at(i)->ShowWindow(SW_SHOW);
        } else {
            vec_dialog_.at(i)->ShowWindow(SW_HIDE);
        }
    }
}

void CConfigDlg::OnBnClickedBtnUser()
{
	for (size_t i = 0; i < vec_dialog_.size(); i++) {
		if (dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i))->IDD == IDD_DLG_USER) {
			vec_dialog_.at(i)->ShowWindow(SW_SHOW);
		} else {
			vec_dialog_.at(i)->ShowWindow(SW_HIDE);
		}
	}
}

void CConfigDlg::OnBnClickedBtnHdd()
{
	for (size_t i = 0; i < vec_dialog_.size(); i++) {
		if (dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i))->IDD == IDD_DLG_HDD) {
			vec_dialog_.at(i)->ShowWindow(SW_SHOW);
		} else {
			vec_dialog_.at(i)->ShowWindow(SW_HIDE);
		}
	}
}

void CConfigDlg::OnBnClickedBtnJson()
{
    // TODO: Add your control notification handler code here
    for (size_t i = 0; i < vec_dialog_.size(); i++) {
        if (dynamic_cast<CQueryAndSetParam*>(vec_dialog_.at(i))->IDD == IDD_DLG_JSON) {
            vec_dialog_.at(i)->ShowWindow(SW_SHOW);
        } else {
            vec_dialog_.at(i)->ShowWindow(SW_HIDE);
        }
    }
}
