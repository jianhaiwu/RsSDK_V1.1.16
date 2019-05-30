#pragma once

#include "ChLiveDlg.h"
#include "UpgradeDlg.h"
#include "UserDlg.h"
#include "HddDlg.h"
#include "JsonPageDlg.h"

// CConfigDlg dialog
class CRSLibDemoDlg;
class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
    void SetParentWindow(CRSLibDemoDlg* parent);

    BOOL                  init_over_;
    CRSLibDemoDlg*        parent_;
    std::vector<CDialog*> vec_dialog_;
    CChLiveDlg            CChLiveDlg_;
    CUpgradeDlg           CUpgradeDlg_;
	CUserDlg              CUserDlg_;
	CHddDlg               CHddDlg_;
    CJsonPageDlg          CJsonPageDlg_;

    afx_msg void OnBnClickedBtnQuery();
    afx_msg void OnBnClickedBtnSet();
    afx_msg void OnBnClickedBtnLiveChannel();
    afx_msg void OnBnClickedBtnupgrade();
	afx_msg void OnBnClickedBtnUser();
	afx_msg void OnBnClickedBtnHdd();
    afx_msg void OnBnClickedBtnJson();
};
