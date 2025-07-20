#include "GameRole.h"
#include "GameMsg.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "ZinxTimer.h"
#include "msg.pb.h"
#include <iostream>

/*创建游戏世界全局对象*/
static AOIWorld world(0, 400, 0, 400, 20, 20);
/*GameRole对象创建时，随机生成合理范围内的坐标。生成随机数的方法:std::default random engine*/
static std::default_random_engine g_random_creator(time(NULL));

class ExitTimer : public TimerOutProc {
    // 通过 TimerOutProc 继承
    virtual void Proc() override {
        ZinxKernel::Zinx_Exit();
    }
    virtual int GetTimerSec() override {
        return 20;
    }
};
static ExitTimer g_exit_timer;

GameRole::GameRole() { 
    szName = "Tom";
    x = g_random_creator() % 20 + 100;
    z = g_random_creator() % 20 + 100;
}

bool GameRole::Init() {
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0) {
        TimerOutMng::GetInstance().DelTask(&g_exit_timer);
    }

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
        for (auto single : srd_list) {   
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

    for (auto single : input.m_Msgs) {
        std::cout << "type is" << single->enMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;

        //取出聊天消息
        switch (single->enMsgType) {
        case GameMsg::MSG_TYPE::MSG_TYPE_CHAT_CONTENT:
        {
            /*取出聊天内容*/
            auto content = dynamic_cast<pb::Talk*>(single->pMsg)->content();
            //发给所有人
            auto role_list = ZinxKernel::Zinx_GetAllRole();
            for (auto pRole : role_list) {
                auto pGameRole = dynamic_cast<GameRole*>(pRole);
                content = this->szName + ": " + content;
                auto pmsg = CreateTalkBroadCast(content);
                ZinxKernel::Zinx_SendOut(*pmsg, *(pGameRole->m_pProto));
            }
        }
        case GameMsg::MSG_TYPE::MSG_TYPE_NEW_POSTION:
        {
            //取出新位置
            auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);

            /*1.跨网格处理*/
            /*获取原来的邻居s1*/
            auto s1 = world.GetSurroundPlayers(this);
            /*摘出旧格子,更新坐标,添加新格子,获取新邻居s2*/
            world.DelPlayer(this);
            x = NewPos->x();
            y = NewPos->y();
            z = NewPos->z();
            v = NewPos->v();
            world.AddPlayer(this);
            auto s2 = world.GetSurroundPlayers(this);
            

            /*2.广播新位置给周围玩家*/
            /*遍历s2, 若元素不属于s1, 视野出现*/
            for (auto single_player : s2) {
                if (s1.end() == std::find(s1.begin(), s1.end(), single_player)) {
                    // 让我的世界里显示对方突然出生
                    auto _pRole = dynamic_cast<GameRole*>(single_player);
                    auto pmsg = _pRole->CreateSelfPostion();
                    ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
                    // 让他的世界里显示我突然出生
                    pmsg = CreateSelfPostion();
                    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));
                }
            }
            /*遍历s1, 若元素不属于s2, 视野消失*/
            for (auto single_player : s1) {
                if (s2.end() == std::find(s2.begin(), s2.end(), single_player)) {
                    // 让我看不到他
                    auto _pRole = dynamic_cast<GameRole*>(single_player);
                    auto pmsg = _pRole->CreateIDNameLogoff();
                    ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
                    // 让他看不到我
                    pmsg = CreateIDNameLogoff();
                    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));
                }
            }

            /*向周围玩家发送自己的位置*/
            //组成待发送的报文
            //遍历周围玩家发送
            auto srd_list = world.GetSurroundPlayers(this);
            for (auto single : srd_list) {
                auto pmsg = CreateSelfPostion(NewPos);
                auto pRole = dynamic_cast<GameRole*>(single);
                ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
            }            
        }
        default:
            break;
        }
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

    world.DelPlayer(this);

    /*判断是否是最后一个玩家--->起定时器*/
    if (ZinxKernel::Zinx_GetAllRole().size() <= 1)
    {
        //起退出定时器
        TimerOutMng::GetInstance().AddTask(&g_exit_timer);
    }
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

GameMsg* GameRole::CreateSelfPostion(pb::Position* NewPos) {
    pb::Broadcast* pMsg = new pb::Broadcast();

    auto pPosition = pMsg->mutable_p();
    pPosition->set_x(x);
    pPosition->set_y(y);
    pPosition->set_z(z);
    pPosition->set_v(v);

    pMsg->set_pid(iPid);
    pMsg->set_username(szName);
    pMsg->set_tp(4);

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

GameMsg* GameRole::CreateTalkBroadCast(std::string _content) {
    pb::Broadcast* pmsg = new pb::Broadcast();
    pmsg->set_pid(iPid);
    pmsg->set_username(szName);
    pmsg->set_tp(1);
    pmsg->set_content(_content);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_BROADCAST, pmsg);
    return pRet;
}