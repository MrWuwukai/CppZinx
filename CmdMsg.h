#pragma once
class CmdMsg :
    public UserData
{
public:
    /*成员变量表示要回显的字符串*/
    std::string szUserData;
    bool isOpen = true; // 开启输出标志
    bool isCmd = false;
    bool isNeedDate = false;
    CmdMsg();
    virtual ~CmdMsg();
};

