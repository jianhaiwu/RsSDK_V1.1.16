#pragma once
#include "QueryAndSetParam.h"

// CJsonPageDlg dialog

class CJsonPageDlg : public CDialog, public CQueryAndSetParam
{
	DECLARE_DYNAMIC(CJsonPageDlg)

public:
	CJsonPageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJsonPageDlg();

// Dialog Data
	enum { IDD = IDD_DLG_JSON };

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
