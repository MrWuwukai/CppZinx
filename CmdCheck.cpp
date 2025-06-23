#include "CmdCheck.h"
#include "CmdMsg.h"
#include "Roles.h"

CmdCheck* CmdCheck::poSingle = new CmdCheck();

CmdCheck::CmdCheck() {
    std::cout << "CmdCheck constructor called" << std::endl;
}
CmdCheck::~CmdCheck() {
    std::cout << "CmdCheck destructor called" << std::endl;
}

// 通过 IProtocol 继承
UserData* CmdCheck::raw2request(std::string _szInput) {  
    if ("exit" == _szInput) {
        ZinxKernel::Zinx_Exit();
        return nullptr;
    }
    auto pret = new CmdMsg();
    pret->szUserData = _szInput; 
    if ("open" == _szInput)
    {
        pret->isCmd = true;
        pret->isOpen = true;
    }
    if ("close" == _szInput)
    {
        pret->isCmd = true;
        pret->isOpen = false;
    }
    if ("date" == _szInput)
    {
        pret->isCmd = true;
        pret->isNeedDate = true;
    }
    if ("cleardate" == _szInput)
    {
        pret->isCmd = true;
        pret->isNeedDate = false;
    }
    return pret;
}

std::string* CmdCheck::response2raw(UserData& _oUserData) {
    GET_REF2DATA(CmdMsg, output, _oUserData);
    return new std::string(output.szUserData);
}

Irole* CmdCheck::GetMsgProcessor(UserDataMsg& _oUserDataMsg) {
    szOutChannel = _oUserDataMsg.szInfo;

    if ("stdin" == szOutChannel)
        szOutChannel = "stdout";

    /*根据命令不同，交个给不同的处理role对象*/
    auto rolelist = ZinxKernel::Zinx_GetAllRole();

    auto pCmdMsg = dynamic_cast<CmdMsg *>(_oUserDataMsg.poUserData);

    bool isCmd = pCmdMsg->isCmd;
    Irole* pRetRole = nullptr;

    for (Irole* prole : rolelist) {
        if (isCmd)
        {
            auto pOutCtrl = dynamic_cast<OutputCtrl*>(prole);
            if (nullptr != pOutCtrl)
            {
                pRetRole = pOutCtrl;
                break;
            }
        }
        else {
            auto pEcho = dynamic_cast<DateRole*>(prole);
            if (nullptr != pEcho)
            {
                pRetRole = pEcho;
                break;
            }
        }
    }
    return pRetRole;
}

Ichannel* CmdCheck::GetMsgSender(BytesMsg& _oBytes) {
    /*指定数据要通过标准输出输出*/
    return ZinxKernel::Zinx_Getchannel_ByInfo(szOutChannel);
}