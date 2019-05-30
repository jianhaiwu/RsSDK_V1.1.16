// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "UserDlg.h"


// CUserDlg dialog

IMPLEMENT_DYNAMIC(CUserDlg, CDialog)

CUserDlg::CUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlg::IDD, pParent)
	, CQueryAndSetParam(CUserDlg::IDD)
	, color_(RGB(rand()%256,rand()%256,rand()%256))
{

}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CUserDlg message handlers

BOOL CUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUserDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc; 
	GetClientRect(rc); 
	pDC->FillSolidRect(rc,color_);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CUserDlg::Query() {
	if (dev_ == 0)
		return FALSE;

	Json::Value value;
	value.clear();
	value["message"] = MsgParamUser;
	value["type"] = QUERY_ALL;
	value["data"] = "";
	Json::StreamWriterBuilder writer;
	std::string param = Json::writeString(writer, value);

	char buf[100*1024] = {0};
	rs_error_code result = rs_query_param(dev_, param.c_str(), buf, _countof(buf));
	if (result == rs_success) value_ = buf;
	TRACE("======<rs_query_param> result:%s\n", value_.c_str());

	return result == rs_success ? TRUE : FALSE;
}

BOOL CUserDlg::Set() {
	if (!value_.empty()) {

		{
			Json::Reader reader;
			Json::Value value;
			if(!reader.parse(value_, value)){
				return false;
			}
			// 设置添加用户
			value[1]["HaveSwitch"] = 1;   
			value[1]["HaveUser"] = 1;
			// value[1]["UserStatus"] = 1;
			value[1]["UserSetRight"] = -1;
			value[1]["Password"] = "123456";
			value[1]["Password2"] = "123456";
			value[1]["c32Password"] = "123456";
			value[1]["c32UserName"] = "admin";
			value[1]["UserName"] = "user1";

			Json::StreamWriterBuilder writer;
			value_ = Json::writeString(writer, value);
		}

		Json::Value value;
		value.clear();
		value["message"] = MsgParamUser;
		value["type"] = SAVE_ALL;
		value["data"] = "";
		Json::StreamWriterBuilder writer;
		std::string param = Json::writeString(writer, value);

		rs_error_code result = rs_set_param(dev_, param.c_str(), value_.c_str());
		TRACE("======<rs_set_param> result:%d\n", result);

		return result == rs_success ? TRUE : FALSE;
	}
	return FALSE;
}
