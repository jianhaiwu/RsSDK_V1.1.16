#pragma once
#include "QueryAndSetParam.h"

// CUpgradeDlg dialog

class CUpgradeDlg : public CDialog, public CQueryAndSetParam
{
	DECLARE_DYNAMIC(CUpgradeDlg)

public:
	CUpgradeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpgradeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_UPGRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

    BOOL Query();
    BOOL Set();

private:
    COLORREF color_;
};
