#include "GameRole.h"
#include "GameMsg.h"
#include <iostream>

bool GameRole::Init()
{
    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, input, _poUserData); // UserData -> MultiMsg

    for (auto single : input.m_Msgs)
    {
        std::cout << "type is" << single->enMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;
    }

    return nullptr;
}

void GameRole::Fini()
{
}