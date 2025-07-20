#pragma once
#include <zinx.h>

#include "AOIWorld.h"

class GameProtocol;// 只声明一下有这个类，只使用指针，这样就避免include
class GameRole : public Irole, public Player {
public:
    GameRole();
    virtual ~GameRole();

    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    GamePrptocol* m_pProto = NULL;
private:
    // init position
    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;
    int iPid = 0;
    std::string szName;
    GameMsg* CreateIDNameLogin();
    GameMsg* CreateSrdPlayers();
    GameMsg* CreateSelfPostion();
    GameMsg* CreateSelfPostion(pb::Position*);
    GameMsg* CreateIDNameLogoff();
    GameMsg* CreateTalkBroadCast(std::string _content);
};

