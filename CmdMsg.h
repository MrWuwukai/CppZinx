#pragma once
class CmdMsg :
    public UserData
{
public:
    /*��Ա������ʾҪ���Ե��ַ���*/
    std::string szUserData;
    bool isOpen = true; // ���������־
    bool isCmd = false;
    bool isNeedDate = false;
    CmdMsg();
    virtual ~CmdMsg();
};

