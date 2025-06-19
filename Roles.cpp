#include "Roles.h"
#include "CmdMsg.h"
#include "CmdCheck.h"

Echo::Echo()
{
}

Echo::~Echo()
{
}

bool Echo::Init()
{
    return true;
}

UserData* Echo::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, input, _poUserData);
    CmdMsg* poOut = new CmdMsg(input);
    ZinxKernel::Zinx_SendOut(poOut, *(Cmdcheck::GetInstance()));
    return nullptr;
}

void Echo::Fini()
{
}

OutputCtrl::OutputCtrl()
{
}

OutputCtrl::~OutputCtrl()
{
}

bool OutputCtrl::Init()
{
	auto rolelist = ZinxKernel::Zinx_GetAllRole();
	Irole* pRetRole = nullptr;

	for (Irole* prole : rolelist) {
		auto p = dynamic_cast<DateRole*>(prole);
		if (nullptr != p)
		{
			pRetRole = p;
			break;
		}
	}

	if (pRetRole != nullptr) {
		SetNextProcessor(*pRetRole);
	}

	return true;
}

UserData* OutputCtrl::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, input, _poUserData);   
	if (true == input.isOpen)
	{
		//开输出
        if (pOut != nullptr) {
            ZinxKernel::Zinx_Add_Channel(*(pOut));
            pOut = nullptr;
        }
	}
    else
    {
        //关输出
        auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
        ZinxKernel::Zinx_Del_Channel(*(pchannel));
        pOut = pchannel;
    }
    return new CmdMsg(input);
}

void OutputCtrl::Fini()
{
}

DateRole::DateRole()
{
}

DateRole::~DateRole()
{
}

bool DateRole::Init()
{
    auto rolelist = ZinxKernel::Zinx_GetAllRole();
    Irole* pRetRole = nullptr;

    for (Irole* prole : rolelist) {
        auto pEcho = dynamic_cast<Echo*>(prole);
        if (nullptr != pEcho)
        {
            pRetRole = pEcho;
            break;
        }
    }

    if (pRetRole != nullptr) {
        SetNextProcessor(*pRetRole);
    }

    return true;
}

UserData* DateRole::ProcMsg(UserData& _poUserData)
{
    CmdMsg* pret = nullptr;
    GET_REF2DATA(CmdMsg, input, _poUserData);
    // 是命令则开关是否输出日期，否则把日期数输出出去
    if (input.isCmd)
    {
        isNeedDate = input.isNeedDate;
    }
    else if (isNeedDate)
    {
        time_t tmp;
        time(&tmp);
        string szNew = string(ctime(&tmp)) + input.szUserData;
        pret = new CmdMsg(input);
        pret->szUserData = szNew;
    }
    else {
        pret = new CmdMsg(input);
    }
    return pret;
}

void DateRole::Fini()
{
}