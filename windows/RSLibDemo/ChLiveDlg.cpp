// hLiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSLibDemo.h"
#include "ChLiveDlg.h"


// CChLiveDlg dialog

IMPLEMENT_DYNAMIC(CChLiveDlg, CDialog)

CChLiveDlg::CChLiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChLiveDlg::IDD, pParent)
    , CQueryAndSetParam(CChLiveDlg::IDD)
    , color_(RGB(rand()%256,rand()%256,rand()%256))
{

}

CChLiveDlg::~CChLiveDlg()
{
}

void CChLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChLiveDlg, CDialog)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CChLiveDlg message handlers

BOOL CChLiveDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChLiveDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rc; 
    GetClientRect(rc); 
    pDC->FillSolidRect(rc,color_);
    return TRUE;
    return CDialog::OnEraseBkgnd(pDC);
}

BOOL CChLiveDlg::Query() {
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

BOOL CChLiveDlg::Set() {
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
