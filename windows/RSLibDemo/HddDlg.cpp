// HddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "HddDlg.h"


// CHddDlg dialog

IMPLEMENT_DYNAMIC(CHddDlg, CDialog)

CHddDlg::CHddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHddDlg::IDD, pParent)
	, CQueryAndSetParam(CHddDlg::IDD)
	, color_(RGB(rand()%256,rand()%256,rand()%256))
{

}

CHddDlg::~CHddDlg()
{
}

void CHddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHddDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CHddDlg message handlers

BOOL CHddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  
}

BOOL CHddDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc; 
	GetClientRect(rc); 
	pDC->FillSolidRect(rc,color_);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CHddDlg::Query() {
	if (dev_ == 0)
		return FALSE;

	Json::Value value;
	value.clear();
	value["message"] = MsgParamHdd;
	value["type"] = QUERY_ALL;
	value["data"] = "";
	Json::StreamWriterBuilder writer;
	std::string param = Json::writeString(writer, value);

	char buf[100*1024] = {0};
	rs_error_code result = rs_query_param(dev_, param.c_str(), buf, _countof(buf));
	if (result == rs_success) value_ = buf;
	TRACE("======<rs_query_param> result:%s\n", value_.c_str());

	Json::CharReaderBuilder reader;
	std::string err = "err";
	Json::Value value_res;
	if(!Json::parseFromStream(reader, buf, &value_res, &err))
		return rs_parse_json_error;
	for(int i = 0; i < value_res.size(); ++i) {
		int HddState = value_res[i]["HddState"].asInt();
		switch(HddState)
		{
		case 0:
			TRACE("====== HddState: none \n");
			break;
		case 1:
			TRACE("====== HddState: unformat \n");
			break;
		case 2:
			TRACE("====== HddState: ok \n");
			break;
		case 3:
			TRACE("====== HddState: full \n");
			break;
		case 4:
			TRACE("====== HddState: read oly \n");
			break;
		case 5:
			TRACE("====== HddState: bad \n");
			break;
		default:
			TRACE("====== HddState: none \n");
			break;
		}
	}
	return result == rs_success ? TRUE : FALSE;
}

BOOL CHddDlg::Set() {
	if (!value_.empty()) {
		{
			Json::Reader reader;
			Json::Value value;
			if(!reader.parse(value_, value)){
				return false;
			}
			value[0]["OverWrite"] = 1;   

			Json::StreamWriterBuilder writer;
			value_ = Json::writeString(writer, value);
		}

		Json::Value value;
		value.clear();
		value["message"] = MsgParamHdd;
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
