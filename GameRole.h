#pragma once
#include <zinx.h>

class GameProtocol;// 只声明一下有这个类，只使用指针，这样就避免include
class GameRole : public Irole {
public:
    GameRole();
    virtual ~GameRole();

    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    GamePrptocol* m_pProto = NULL;
};

