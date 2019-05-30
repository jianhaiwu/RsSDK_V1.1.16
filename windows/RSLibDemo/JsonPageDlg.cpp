// JsonPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "JsonPageDlg.h"


// CJsonPageDlg dialog

IMPLEMENT_DYNAMIC(CJsonPageDlg, CDialog)

CJsonPageDlg::CJsonPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJsonPageDlg::IDD, pParent)
    , CQueryAndSetParam(CJsonPageDlg::IDD)
    , color_(RGB(rand()%256,rand()%256,rand()%256))
{

}

CJsonPageDlg::~CJsonPageDlg()
{
}

void CJsonPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJsonPageDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CJsonPageDlg message handlers

BOOL CJsonPageDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CJsonPageDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rc; 
    GetClientRect(rc); 
    pDC->FillSolidRect(rc,color_);
    return TRUE;
    return CDialog::OnEraseBkgnd(pDC);
}

BOOL CJsonPageDlg::Query() {
    if (dev_ == 0)
        return FALSE;

    Json::Value value;
    value.clear();
    value["message"] = MsgParamModifyLiving;
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

BOOL CJsonPageDlg::Set() {
    if (!value_.empty()) {
        {
            std::string err = "";
            Json::CharReaderBuilder reader;
            Json::Value value;
            if(!Json::parseFromStream(reader, value_.c_str(), &value, &err))
                return FALSE;

            //do something
            Json::StreamWriterBuilder writer;
            value_ = Json::writeString(writer, value);
        }

        Json::Value value;
        value.clear();
        value["message"] = MsgParamModifyLiving;
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
