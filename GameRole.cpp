#include "GameRole.h"
#include "GameMsg.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "ZinxTimer.h"
#include "msg.pb.h"
#include <iostream>

/*������Ϸ����ȫ�ֶ���*/
static AOIWorld world(0, 400, 0, 400, 20, 20);
/*GameRole���󴴽�ʱ��������ɺ���Χ�ڵ����ꡣ����������ķ���:std::default random engine*/
static std::default_random_engine g_random_creator(time(NULL));

class ExitTimer : public TimerOutProc {
    // ͨ�� TimerOutProc �̳�
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

    /*����Լ�����Ϸ����*/
    bool bRet = false;
    /*�������IDΪ��ǰ���ӵ�fd��ȷ��ID���ظ�*/
    iPid = m_pProto->m_channel->GetFd();
    bRet = world.AddPlayer(this);

    if (true == bRet) {
        /*���Լ�����ID������*/
        auto pmsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
        /*���Լ�������Χ��ҵ�λ��*/
        pmsg = CreateSrdPlayers();
        ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
        /*����Χ��ҷ����Լ���λ��*/
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

        //ȡ��������Ϣ
        switch (single->enMsgType) {
        case GameMsg::MSG_TYPE::MSG_TYPE_CHAT_CONTENT:
        {
            /*ȡ����������*/
            auto content = dynamic_cast<pb::Talk*>(single->pMsg)->content();
            //����������
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
            //ȡ����λ��
            auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);

            /*1.��������*/
            /*��ȡԭ�����ھ�s1*/
            auto s1 = world.GetSurroundPlayers(this);
            /*ժ���ɸ���,��������,����¸���,��ȡ���ھ�s2*/
            world.DelPlayer(this);
            x = NewPos->x();
            y = NewPos->y();
            z = NewPos->z();
            v = NewPos->v();
            world.AddPlayer(this);
            auto s2 = world.GetSurroundPlayers(this);
            

            /*2.�㲥��λ�ø���Χ���*/
            /*����s2, ��Ԫ�ز�����s1, ��Ұ����*/
            for (auto single_player : s2) {
                if (s1.end() == std::find(s1.begin(), s1.end(), single_player)) {
                    // ���ҵ���������ʾ�Է�ͻȻ����
                    auto _pRole = dynamic_cast<GameRole*>(single_player);
                    auto pmsg = _pRole->CreateSelfPostion();
                    ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
                    // ��������������ʾ��ͻȻ����
                    pmsg = CreateSelfPostion();
                    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));
                }
            }
            /*����s1, ��Ԫ�ز�����s2, ��Ұ��ʧ*/
            for (auto single_player : s1) {
                if (s2.end() == std::find(s2.begin(), s2.end(), single_player)) {
                    // ���ҿ�������
                    auto _pRole = dynamic_cast<GameRole*>(single_player);
                    auto pmsg = _pRole->CreateIDNameLogoff();
                    ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
                    // ������������
                    pmsg = CreateIDNameLogoff();
                    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));
                }
            }

            /*����Χ��ҷ����Լ���λ��*/
            //��ɴ����͵ı���
            //������Χ��ҷ���
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
    /*����Χ��ҷ���������Ϣ*/
    auto srd_list = world.GetSurroundPlayers(this);
    for (auto single : srd_list)
    {
        auto pMsg = CreateIDNameLogoff();
        auto pRole = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_pProto));
    }

    world.DelPlayer(this);

    /*�ж��Ƿ������һ�����--->��ʱ��*/
    if (ZinxKernel::Zinx_GetAllRole().size() <= 1)
    {
        //���˳���ʱ��
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
        auto pPlayer = pMsg->add_ps(); // �ӵ�������protobuf
        auto pRole = dynamic_cast<GameRole*>(single);
        pPlayer->set_pid(pRole->iPid);
        pPlayer->set_username(pRole->szName);
        auto pPostion = pPlayer->mutable_p(); // �ӵ�Ƕ����protobuf
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