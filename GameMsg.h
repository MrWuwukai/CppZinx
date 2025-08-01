#pragma once
#include <zinx.h>
#include <google/protobuf/message.h>
#include <list>
class GameMsg :
    public UserData
{
public:
    /*用户的请求信息*/
    google::protobuf::Message* pMsg = NULL;
    enum class MSG_TYPE {
        MSG_TYPE_LOGIN_ID_NAME = 1,
        MSG_TYPE_CHAT_CONTENT = 2,
        MSG_TYPE_NEW_POSTION = 3,
        MSG_TYPE_BROADCAST = 200,
        MSG_TYPE_LOGOFF_ID_NAME = 201,
        MSG_TYPE_SRD_POSTION = 202
    } enMsgType;

    GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg);
    GameMsg(MSG_TYPE type, std::string _stream);
    ~GameMsg();

    /*序列化本消息*/
    std::string serialize();

    virtual ~GameMsg();
};

class MultiMsg :public UserData {
public:
    std::list<GameMsg*> m_Msgs;
};

