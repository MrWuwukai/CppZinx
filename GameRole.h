#pragma once
#include <zinx.h>

class GameProtocol;// ֻ����һ��������ֻ࣬ʹ��ָ�룬�����ͱ���include
class GameRole : public Irole {
public:
    GameRole();
    virtual ~GameRole();

    // ͨ�� Irole �̳�
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    GamePrptocol* m_pProto = NULL;
};

