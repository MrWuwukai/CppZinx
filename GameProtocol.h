#pragma once
#include <zinx.h>

class GameChannel;
class GameRole;
class GameProtocol : public Iprotocol {
public:
    GameProtocol();
    virtual ~GameProtocol();

    // ͨ�� Iprotocol �̳�
    virtual UserData* raw2request(std::string _szInput) override;
    virtual std::string* response2raw(UserData& _oUserData) override;
    virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

    GameChannel* m_channel = NULL;
    GameRole* m_role = NULL;
private:
    std::string szTcpBuffer; // �����
};

