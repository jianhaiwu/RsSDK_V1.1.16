#pragma once
#include "QueryAndSetParam.h"


// CHddDlg dialog

class CHddDlg : public CDialog, public CQueryAndSetParam
{
	DECLARE_DYNAMIC(CHddDlg)

public:
	CHddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHddDlg();

// Dialog Data
	enum { IDD = IDD_DLG_HDD };

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
