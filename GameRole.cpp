#include "GameRole.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include <iostream>

/*������Ϸ����ȫ�ֶ���*/
static AOIWorld world(0, 400, 0, 400, 20, 20);

bool GameRole::Init() {
    /*����Լ�����Ϸ����*/
    bool bRet = false;
    bRet = world.AddPlayer(this);
    return bRet;
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

void GameRole::Fini(){
    world.AddPlayer(this);
}

int GameRole::GetX(){
    return (int)x;
}

int GameRole::GetY(){
    return (int)z;
}