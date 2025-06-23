#pragma once
class CmdCheck :
    public Iprotocol
{
    CmdCheck();
    virtual ~CmdCheck();
    static CmdCheck* poSingle;
public:
    // Í¨¹ý IProtocol ¼Ì³Ð
    virtual UserData* raw2request(std::string _szInput);
    virtual std::string* response2raw(UserData& _oUserData);
    virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg);
    virtual Ichannel* GetMsgSender(BytesMsg& _oBytes);
    static CmdCheck* GetInstance() {
        return poSingle;
    }

    std::string szOutChannel;
};

