#include "GameRole.h"
#include "GameMsg.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "msg.pb.h"
#include <iostream>

/*创建游戏世界全局对象*/
static AOIWorld world(0, 400, 0, 400, 20, 20);

GameRole::GameRole() { 
    szName = "Tom";
}

bool GameRole::Init() {
    /*添加自己到游戏世界*/
    bool bRet = false;
    /*设置玩家ID为当前连接的fd，确保ID不重复*/
    iPid = m_pProto->m_channel->GetFd();
    bRet = world.AddPlayer(this);

    if (true == bRet) {
        /*向自己发送ID和名称*/
        auto pmsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
        /*向自己发送周围玩家的位置*/
        pmsg = CreateSrdPlayers();
        ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
        /*向周围玩家发送自己的位置*/
        auto srd_list = world.GetSurroundPlayers(this);
        for (auto single : srd_list)
        {
            pmsg = CreateSelfPostion();
            auto pRole = dynamic_cast<GameRole*>(single);
            ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
        }
    }
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
    /*向周围玩家发送下线消息*/
    auto srd_list = world.GetSurroundPlayers(this);
    for (auto single : srd_list)
    {
        auto pMsg = CreateIDNameLogoff();
        auto pRole = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_pProto));
    }

    world.AddPlayer(this);
}

int GameRole::GetX(){
    return (int)x;
}

int GameRole::GetY(){
    return (int)z;
}

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pmsg = new pb::SyncPid();
    pmsg->set_pid(iPid);
    pmsg->set_username(szName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_LOGIN_ID_NAME, pmsg);
    return pRet;
}

GameMsg* GameRole::CreateSrdPlayers() {
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    auto srd_list = world.GetSurroundPlayers(this);

    for (auto single : srd_list)
    {
        auto pPlayer = pMsg->add_ps(); // 加到数组型protobuf
        auto pRole = dynamic_cast<GameRole*>(single);
        pPlayer->set_pid(pRole->iPid);
        pPlayer->set_username(pRole->szName);
        auto pPostion = pPlayer->mutable_p(); // 加到嵌套型protobuf
        pPostion->set_x(pRole->x);
        pPostion->set_y(pRole->y);
        pPostion->set_z(pRole->z);
        pPostion->set_v(pRole->v);
    }
    GameMsg* pret = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_SRD_POSTION, pMsg);
    return pret;
}

GameMsg* GameRole::CreateSelfPostion() {
    pb::Broadcast* pMsg = new pb::Broadcast();
    pMsg->set_pid(iPid);
    pMsg->set_username(szName);
    pMsg->set_tp(2);

    auto pPosition = pMsg->mutable_p();
    pPosition->set_x(x);
    pPosition->set_y(y);
    pPosition->set_z(z);
    pPosition->set_v(v);

    GameMsg* pret = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_BROADCAST, pMsg);
    return pret;
}

GameMsg* GameRole::CreateIDNameLogoff() {
    pb::SyncPid* pmsg = new pb::SyncPid();
    pmsg->set_pid(iPid);
    pmsg->set_username(szName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
    return pRet;
}