#pragma once
#include "QueryAndSetParam.h"

// CChLiveDlg dialog

class CChLiveDlg : public CDialog, public CQueryAndSetParam
{
	DECLARE_DYNAMIC(CChLiveDlg)

public:
	CChLiveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChLiveDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CHLIVE };

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
