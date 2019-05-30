#pragma once
#include "QueryAndSetParam.h"

// CUserDlg dialog

class CUserDlg : public CDialog, public CQueryAndSetParam
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserDlg();

// Dialog Data
	enum { IDD = IDD_DLG_USER };

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
