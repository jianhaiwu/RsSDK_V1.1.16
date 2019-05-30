#pragma once

class CQueryAndSetParam
{
public:
    CQueryAndSetParam(int IDD_) : IDD(IDD_), dev_(0) {}
    virtual ~CQueryAndSetParam(void) {}

    virtual BOOL Query() = 0;
    virtual BOOL Set() = 0;

public:
    int         IDD;
    device_id   dev_;
    std::string value_;
};
